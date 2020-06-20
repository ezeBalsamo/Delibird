#include "../include/pokemon_occurrences.h"
#include "../../Utils/include/matrix.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/pthread_wrapper.h"
#include <commons/string.h>
#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/dictionary.h>
#include <pokemon_occurrence_trigger.h>
#include <team_logs_manager.h>
#include <appeared_query_performer.h>

t_dictionary* targetable_pokemons_by_name;

pthread_mutex_t ocurrences_mutex;
extern pthread_mutex_t targetable_status_mutex;
sem_t trainer_thread_context_has_become_blocked_semaphore;

void free_targetable_pokemon(t_targetable_object* targetable_pokemon){
    t_localizable_object* localizable_pokemon = targetable_pokemon -> localizable_pokemon;
    free(localizable_pokemon);
    free(targetable_pokemon);
}

t_list* occurrences_of(char* pokemon_name){
    char* uppercase_pokemon_name = string_duplicate(pokemon_name);
    string_to_upper(uppercase_pokemon_name);

    t_list* targetable_pokemons_found = dictionary_get(targetable_pokemons_by_name, uppercase_pokemon_name);
    free(uppercase_pokemon_name);

    if(!targetable_pokemons_found){
        log_pokemon_not_belonging_to_global_goal_error_for(pokemon_name);
        free_system();
    }

    return targetable_pokemons_found;
}

void add_occurrence_of(t_targetable_object* targetable_pokemon){
    
    char* pokemon_name = targetable_pokemon -> localizable_pokemon -> object;
    t_list* targetable_pokemons = occurrences_of(pokemon_name);
    list_add(targetable_pokemons, targetable_pokemon);
}

bool is_of(t_targetable_object* targetable_pokemon, t_localizable_object* localizable_pokemon){
    t_localizable_object* targetable_localizable_pokemon = targetable_pokemon -> localizable_pokemon;
    
    return 
        targetable_localizable_pokemon -> object == localizable_pokemon -> object
        && targetable_localizable_pokemon -> pos_x == localizable_pokemon -> pos_x
        && targetable_localizable_pokemon -> pos_y == localizable_pokemon -> pos_y;
}

bool is_not_targeted(void* targetable_pokemon){
    return !(((t_targetable_object*) targetable_pokemon) -> is_being_targeted);
}

t_list* not_yet_targeted_pokemons(){
    t_list* not_yet_targeted_pokemons = list_create();

    void _load_not_yet_targeted_pokemons(char* pokemon_name, void* targetable_pokemons){
        (void) pokemon_name;

        t_list* filtered_not_yet_targeted_pokemons = list_filter((t_list*) targetable_pokemons, is_not_targeted);
        list_add_all(not_yet_targeted_pokemons, filtered_not_yet_targeted_pokemons);

        list_destroy(filtered_not_yet_targeted_pokemons);
    }

    dictionary_iterator(targetable_pokemons_by_name, _load_not_yet_targeted_pokemons);
    return not_yet_targeted_pokemons;
}

void new_occurrence_of(t_targetable_object* targetable_pokemon){

    pthread_mutex_lock(&ocurrences_mutex);
    add_occurrence_of(targetable_pokemon);
    pthread_mutex_unlock(&ocurrences_mutex);
    pthread_mutex_unlock(&targetable_status_mutex);

    if(targetable_pokemon -> is_being_targeted){
        chase(targetable_pokemon -> localizable_pokemon);
    }
}

uint32_t occurrences_of_pokemon_named(char* pokemon_name){
    t_list* targetable_pokemons = occurrences_of(pokemon_name);
    return list_size(targetable_pokemons);
}

void consider_become_targetable_next_pokemon_named(char* pokemon_name){

    if (amount_required_of(pokemon_name) > 0){

        t_list* targetable_pokemons = occurrences_of(pokemon_name);

        if (!list_is_empty(targetable_pokemons)){
            t_targetable_object* targetable_pokemon = list_get(targetable_pokemons, 0);
            targetable_pokemon -> is_being_targeted = true;
            sem_wait(&trainer_thread_context_has_become_blocked_semaphore);
            chase(targetable_pokemon -> localizable_pokemon);
        }
    }
}

void remove_occurrence_of(t_localizable_object* localizable_pokemon){

    char* pokemon_name = localizable_pokemon -> object;

    t_list* targetable_pokemons = occurrences_of(pokemon_name);

    bool _is_of(void* targetable_pokemon){
        return is_of(targetable_pokemon, localizable_pokemon);
    }

    t_targetable_object* targetable_pokemon_found = list_remove_by_condition(targetable_pokemons, _is_of);

    if(!targetable_pokemon_found){
        log_targetable_pokemon_not_found_error_for(localizable_pokemon);
        free_system();
    }

    free_targetable_pokemon(targetable_pokemon_found);

    consider_become_targetable_next_pokemon_named(pokemon_name);
}

void initialize_occurrence_of(t_pokemon_goal* pokemon_goal){
    
    t_list* targetable_pokemons = list_create();
    char* pokemon_name = string_duplicate(pokemon_goal -> pokemon_name);
    string_to_upper(pokemon_name);
    dictionary_put(targetable_pokemons_by_name, pokemon_name, targetable_pokemons);
    free(pokemon_name);
}

void initialize_pokemon_occurrences(){

    targetable_pokemons_by_name = dictionary_create();

    safe_mutex_initialize(&ocurrences_mutex);
    sem_initialize(&trainer_thread_context_has_become_blocked_semaphore);

    with_global_goal_do(initialize_occurrence_of);
}

void free_targetable_pokemons(t_list* targetable_pokemons){
    list_destroy_and_destroy_elements(targetable_pokemons, (void (*)(void *)) free_targetable_pokemon);
}

void free_pokemon_occurrences(){
    dictionary_destroy_and_destroy_elements(targetable_pokemons_by_name, (void (*)(void *)) free_targetable_pokemons);
    pthread_mutex_destroy(&ocurrences_mutex);
}