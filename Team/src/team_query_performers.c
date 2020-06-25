#include <team_manager.h>
#include <pokemon_occurrences.h>
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/pthread_wrapper.h"
#include <stdlib.h>
#include <team_query_performers.h>
#include <appeared_query_performer.h>
#include <localized_query_performer.h>
#include <caught_query_performer.h>
#include <team_logs_manager.h>
#include <pthread.h>
#include <commons/collections/dictionary.h>
#include <commons/string.h>

t_list* query_performers;
t_dictionary* pokemon_occurrence_status_by_name;

pthread_mutex_t pokemon_occurrence_status_by_name_mutex;
pthread_mutex_t targetable_status_mutex;

bool* occurrence_status_of(char* pokemon_name){
    char* pokemon_name_as_uppercase = string_duplicate(pokemon_name);
    string_to_upper(pokemon_name_as_uppercase);

    bool* status = dictionary_get(pokemon_occurrence_status_by_name, pokemon_name_as_uppercase);

    free(pokemon_name_as_uppercase);
    return status;
}

bool exists_previous_occurrence_of(char* pokemon_name){
    bool* status = occurrence_status_of(pokemon_name);
    return *status;
}

void mark_occurrence_status_of(char* pokemon_name){
    bool* status = occurrence_status_of(pokemon_name);
    *status = true;
}

t_request* internal_request_from(t_request* deserialized_request){

    t_identified_message* correlative_identified_message = (t_identified_message*) deserialized_request -> structure;
    t_identified_message* original_identified_message = internal_object_in(correlative_identified_message);

    return original_identified_message -> request;
}

void initialize_and_load_query_performers(){

    initialize_appeared_query_performer();
    initialize_localized_query_performer();
    initialize_caught_query_performer();

    list_add(query_performers, (void*) appeared_query_performer());
    list_add(query_performers, (void*) localized_query_performer());
    list_add(query_performers, (void*) caught_query_performer());
}

void initialize_occurrence_status_of(t_pokemon_goal* pokemon_goal){

    char* pokemon_name = string_duplicate(pokemon_goal -> pokemon_name);
    string_to_upper(pokemon_name);

    bool* status = safe_malloc(sizeof(bool));
    *status = false;

    dictionary_put(pokemon_occurrence_status_by_name, pokemon_name, status);
    free(pokemon_name);
}

void initialize_team_query_performers(){

    safe_mutex_initialize(&targetable_status_mutex);
    safe_mutex_initialize(&pokemon_occurrence_status_by_name_mutex);

    query_performers = list_create();
    initialize_and_load_query_performers();

    pokemon_occurrence_status_by_name = dictionary_create();
    with_global_goal_do(initialize_occurrence_status_of);
}

t_query_performer* query_performer_handle(uint32_t operation){

    bool _can_handle(void* query_performer){
        t_query_performer* cast_query_performer = (t_query_performer*) query_performer;
        return (*(cast_query_performer -> can_handle_function)) (operation);
    }

    t_query_performer* query_performer_found = list_find(query_performers, _can_handle);

    if (!query_performer_found){
        log_query_performer_not_found_error_for(operation);
        free_system();
    }

    return query_performer_found;
}

void query_perform(t_request* request){
    t_request* parse_request = internal_request_from(request);
    t_query_performer* query_performer = query_performer_handle(parse_request -> operation);
    query_performer -> perform_function (request -> structure);
}

bool should_be_targeted_pokemon_named(char* pokemon_name){
    uint32_t amount_required = amount_required_of(pokemon_name);
    uint32_t ocurrences = occurrences_of_pokemon_named(pokemon_name);
    return ocurrences < amount_required;
}

t_targetable_object* targetable_pokemon_according_to(char* pokemon_name, uint32_t pos_x, uint32_t pos_y){

    t_localizable_object* localizable_pokemon = safe_malloc(sizeof(t_localizable_object));
    localizable_pokemon -> pos_x = pos_x;
    localizable_pokemon -> pos_y = pos_y;
    localizable_pokemon -> object = pokemon_name;

    pthread_mutex_lock(&targetable_status_mutex);
    bool should_be_targeted = should_be_targeted_pokemon_named(pokemon_name);

    t_targetable_object* targetable_pokemon = safe_malloc(sizeof(t_targetable_object));
    targetable_pokemon -> should_be_targeted = should_be_targeted;
    targetable_pokemon -> is_being_targeted = false;
    targetable_pokemon -> localizable_pokemon = localizable_pokemon;

    return targetable_pokemon;
}

void free_team_query_performers(){
    dictionary_destroy_and_destroy_elements(pokemon_occurrence_status_by_name, free);
    list_destroy_and_destroy_elements(query_performers, free);

    pthread_mutex_destroy(&targetable_status_mutex);
    pthread_mutex_destroy(&pokemon_occurrence_status_by_name_mutex);

    free_localized_query_performer();
}