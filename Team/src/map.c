#include "../include/map.h"
#include "../../Utils/include/matrix.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/free_system.h"
#include "../../Utils/include/pthread_wrapper.h"
#include <commons/string.h>
#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/dictionary.h>
#include <map_update_trigger.h>
#include <team_logs_manager.h>

t_matrix* map;
t_dictionary* pokemon_occurrences;

pthread_mutex_t map_mutex;
pthread_mutex_t ocurrences_mutex;

t_list* occurrences_of(char* pokemon_name){
    char* uppercase_pokemon_name = string_duplicate(pokemon_name);
    string_to_upper(uppercase_pokemon_name);

    t_list* targetable_pokemons_found = dictionary_get(pokemon_occurrences, uppercase_pokemon_name);
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

void remove_occurence_of(t_localizable_object* localizable_pokemon){
    
    char* pokemon_name = localizable_pokemon -> object;
    t_list* targetable_pokemons = occurrences_of(pokemon_name);
    
    bool _is_of(void* targetable_pokemon){
        return is_of(targetable_pokemon, localizable_pokemon);
    }
    
    t_targetable_object* targetable_pokemon = list_remove_by_condition(targetable_pokemons, _is_of);
}

bool is_not_targeted(void* targetable_pokemon){
    return !(((t_targetable_object*) targetable_pokemon) -> is_being_targeted);
}

t_list* not_yet_targeted_pokemons(){
    t_list* not_yet_targeted_pokemons = list_create();

    void _load_not_yet_targeted_pokemons(char* pokemon_name, void* targetable_pokemons){
        t_list* filtered_not_yet_targeted_pokemons = list_filter((t_list*) targetable_pokemons, is_not_targeted);
        list_add_all(not_yet_targeted_pokemons, filtered_not_yet_targeted_pokemons);
        list_destroy(filtered_not_yet_targeted_pokemons);
    }

    dictionary_iterator(pokemon_occurrences, _load_not_yet_targeted_pokemons);
    return not_yet_targeted_pokemons;
}


void load_trainer_in_map(t_localizable_object* localizable_trainer){
    matrix_insert_element_at(map, (t_trainer *) localizable_trainer -> object, localizable_trainer -> pos_x,
                             localizable_trainer -> pos_y);
}
void load_pokemon_in_map(t_targetable_object* targetable_pokemon){

    t_localizable_object* localizable_pokemon = targetable_pokemon -> localizable_pokemon;
    char* pokemon_name = localizable_pokemon -> object;

    pthread_mutex_lock(&map_mutex);
    matrix_insert_element_at(map, pokemon_name, localizable_pokemon -> pos_x, localizable_pokemon -> pos_y);
    pthread_mutex_unlock(&map_mutex);

    pthread_mutex_lock(&ocurrences_mutex);
    add_occurrence_of(targetable_pokemon);
    pthread_mutex_unlock(&ocurrences_mutex);
    
    if(targetable_pokemon -> is_being_targeted){
        map_updated_with_insertion_of(localizable_pokemon);
    }
}

uint32_t occurrences_amount_in_map_of(char* pokemon_name){
    t_list* targetable_pokemons = occurrences_of(pokemon_name);
    return list_size(targetable_pokemons);
}

void remove_pokemon_from_map(t_localizable_object* localizable_pokemon){
    char* pokemon_name = matrix_remove_element_at(map,
            localizable_pokemon -> pos_x, localizable_pokemon -> pos_y);

    if(!string_equals_ignore_case(localizable_pokemon -> object, pokemon_name)){
        log_incorrent_pokemon_removed_error_for(localizable_pokemon -> object, pokemon_name);
        free_system();
    }

    remove_occurence_of(localizable_pokemon);
    free(localizable_pokemon);
}

void* cast_uint_max_between(void* number, void* another_number){
    uint32_t cast_number =  *((uint32_t*) number);
    uint32_t cast_another_number = *((uint32_t*) another_number);
    return cast_number > cast_another_number ? number : another_number;
}

uint32_t furthest_trainer_position(){
    uint32_t null_position = 0;

    t_list* x_positions = trainers_x_positions();
    void* max_x_position = list_fold(x_positions, (void *) &null_position, cast_uint_max_between);

    t_list* y_positions = trainers_y_positions();
    void* max_y_position = list_fold(y_positions, (void *) &null_position, cast_uint_max_between);

    uint32_t furthest_position = *((uint32_t*) cast_uint_max_between(max_x_position, max_y_position));

    list_destroy(x_positions);
    list_destroy(y_positions);

    return furthest_position;
}

void initialize_occurrence_of(t_pokemon_goal* pokemon_goal){
    
    t_list* targetable_pokemons = list_create();
    char* pokemon_name = string_duplicate(pokemon_goal -> pokemon_name);
    string_to_upper(pokemon_name);
    dictionary_put(pokemon_occurrences, pokemon_name, targetable_pokemons);
    free(pokemon_name);
}

void initialize_map(){
    uint32_t map_size = furthest_trainer_position();
    map = matrix_create_of_size(map_size, true, false);
    pokemon_occurrences = dictionary_create();

    safe_mutex_initialize(&map_mutex);
    safe_mutex_initialize(&ocurrences_mutex);

    with_global_goal_do(initialize_occurrence_of);
    with_trainers_do(load_trainer_in_map);
}

void matrix_print_trainer(void* trainer){
    char* printable_trainer = string_new();
    uint32_t trainer_sequential_number = ((t_trainer*) trainer) -> sequential_number;
    string_append(&printable_trainer, "E");
    string_append(&printable_trainer, string_itoa(trainer_sequential_number));

    printf("%10s", printable_trainer);
    free(printable_trainer);
}

void free_targetable_pokemon(t_targetable_object* targetable_pokemon){
    t_localizable_object* localizable_pokemon = targetable_pokemon -> localizable_pokemon;
    free(localizable_pokemon);
    free(targetable_pokemon);
}

void free_targetable_pokemons(t_list* targetable_pokemons){
    list_destroy_and_destroy_elements(targetable_pokemons, (void (*)(void *)) free_targetable_pokemon);
}

void free_map(){
    dictionary_destroy_and_destroy_elements(pokemon_occurrences, (void (*)(void *)) free_targetable_pokemons);
    matrix_destroy(map);
}