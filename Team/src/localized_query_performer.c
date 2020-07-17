#include <team_query_performers.h>
#include "localized_query_performer.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/garbage_collector.h"
#include <stdlib.h>
#include <commons/string.h>
#include <team_logs_manager.h>
#include <pokemon_occurrences.h>

t_query_performer* localized_pokemon_query_performer;
t_list* get_responses;

pthread_mutex_t get_responses_mutex;
extern pthread_mutex_t pokemon_occurrence_status_by_name_mutex;

t_query_performer* localized_query_performer(){
    return localized_pokemon_query_performer;
}

t_get_response* get_response_of(uint32_t message_id){

    bool _is_for(t_get_response* get_response){
        return get_response -> response_id == message_id;
    }

    return list_find(get_responses, (bool (*)(void *)) _is_for);
}

bool is_required(uint32_t message_id){

    return get_response_of(message_id) != NULL;
}

void assert_matching_pokemon_name_for(t_identified_message* identified_message){

    t_get_response* get_response = get_response_of(identified_message -> message_id);
    t_localized_pokemon* localized_pokemon = internal_object_in(identified_message);

    char* localized_pokemon_name = localized_pokemon -> pokemon_name;
    char* get_response_pokemon_name = get_response -> pokemon_name;

    if(!string_equals_ignore_case(get_response_pokemon_name, localized_pokemon_name)){

        log_not_matching_pokemon_name_between_get_and_localized_error(
                get_response_pokemon_name,localized_pokemon_name);

        free_system();
    }
}

void add_new_occurrences_according_to(t_localized_pokemon* localized_pokemon){

    char* pokemon_name = localized_pokemon -> pokemon_name;
    int positions_quantity = (int) localized_pokemon -> quantity * 2;

    for(int i = 0; i < positions_quantity; i += 2){

        uint32_t pos_x = *((uint32_t*) list_get(localized_pokemon -> positions, i));
        uint32_t pos_y = *((uint32_t*) list_get(localized_pokemon -> positions, i + 1));

        t_targetable_object* targetable_pokemon = targetable_pokemon_according_to(pokemon_name, pos_x, pos_y);
        new_occurrence_of(targetable_pokemon);
    }
}

void consider_as_new_if_necessary_occurrences_of(t_localized_pokemon* localized_pokemon){

    char* pokemon_name = localized_pokemon -> pokemon_name;

    safe_mutex_lock(&pokemon_occurrence_status_by_name_mutex);

    if(exists_previous_occurrence_of(pokemon_name)){
        log_message_ignored_due_to_previous_existing_occurrences_for(localized_pokemon);
        safe_mutex_unlock(&pokemon_occurrence_status_by_name_mutex);
    }else{
        mark_occurrence_status_of(pokemon_name);
        safe_mutex_unlock(&pokemon_occurrence_status_by_name_mutex);
        add_new_occurrences_according_to(localized_pokemon);
    }
}

void localized_query_performer_function(t_identified_message* correlative_identified_message){

    t_identified_message* identified_message = internal_object_in(correlative_identified_message);
    uint32_t message_id = identified_message -> message_id;

    if(is_required(message_id)){
        assert_matching_pokemon_name_for(identified_message);
        t_localized_pokemon* localized_pokemon = internal_object_in(identified_message);
        consider_as_new_if_necessary_occurrences_of(localized_pokemon);
    }else{
        log_message_id_not_required(LOCALIZED_POKEMON, message_id);
    }
}

void get_pokemon_sent_successfully(t_get_response* get_response){
    safe_mutex_lock(&get_responses_mutex);
    list_add(get_responses, get_response);
    safe_mutex_unlock(&get_responses_mutex);
}

bool localized_query_performer_can_handle(uint32_t operation){
    return operation == LOCALIZED_POKEMON;
}

void initialize_localized_query_performer(){

    get_responses = list_create();
    safe_mutex_initialize(&get_responses_mutex);

    localized_pokemon_query_performer = safe_malloc(sizeof(t_query_performer));
    localized_pokemon_query_performer -> can_handle_function = localized_query_performer_can_handle;
    localized_pokemon_query_performer -> perform_function = localized_query_performer_function;
}

void free_localized_query_performer(){
    list_destroy_and_destroy_elements(get_responses, free);
}
