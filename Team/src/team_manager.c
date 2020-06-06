#include <trainers_parser.h>
#include <goal_calculator.h>
#include <team_manager.h>
#include <map.h>
#include <stdlib.h>
#include <trainer_threads.h>
#include <commons/string.h>
#include <team_logs_manager.h>
#include "../../Utils/include/free_system.h"

t_list* localized_trainers;
t_list* global_goal;

void* initialize_team_manager(){
    localized_trainers = parsed_trainers();
    global_goal = team_global_goal_according_to(localized_trainers);
    initialize_map();
    initialize_trainer_threads();

    return NULL;
}

bool are_equal_trainers(t_trainer* trainer, t_trainer* another_trainer){
    return trainer -> sequential_number == another_trainer -> sequential_number;
}

uint32_t amount_required_of(char* pokemon_name){

    bool _are_equals(void* pokemon_goal){
        t_pokemon_goal* cast_pokemon_goal = (t_pokemon_goal*) pokemon_goal;
        return string_equals_ignore_case(cast_pokemon_goal -> pokemon_name, pokemon_name);
    }

    t_pokemon_goal* pokemon_goal_found = list_find(global_goal, _are_equals);

    if(!pokemon_goal_found){
        log_pokemon_not_belonging_to_global_goal_error_for(pokemon_name);
        free_system();
    }

    return pokemon_goal_found -> quantity;
}

bool global_goal_contains(char* pokemon_name){

    bool _are_equals(void* pokemon_goal){
        t_pokemon_goal* cast_pokemon_goal = (t_pokemon_goal*) pokemon_goal;
        return string_equals_ignore_case(cast_pokemon_goal -> pokemon_name, pokemon_name);
    }

    return list_any_satisfy(global_goal, _are_equals);
}

void update_current_pokemons_after_catching(t_localizable_object* localizable_trainer, char* pokemon_name){
    t_trainer* trainer = localizable_trainer -> object;
    list_add(trainer -> current_pokemons, pokemon_name);
}

t_list* trainers_x_positions(){
    void* _x_position_of(void* localized_trainer){
        return (void*) &(((t_localizable_object*) localized_trainer) -> pos_x);
    }

    return list_map(localized_trainers, _x_position_of);
}

t_list* trainers_y_positions(){
    void* _y_position_of(void* localized_trainer){
        return (void*) &(((t_localizable_object*) localized_trainer) -> pos_y);
    }

    return list_map(localized_trainers, _y_position_of);
}

void with_trainers_do(void (*closure) (t_localizable_object*)){
    list_iterate(localized_trainers, (void (*)(void *)) closure);
}

void with_global_goal_do(void (*closure) (t_pokemon_goal*)){
    list_iterate(global_goal, (void (*)(void *)) closure);
}

void free_trainer(t_trainer *trainer){
    list_destroy_and_destroy_elements(trainer -> required_pokemons, free);
    list_destroy_and_destroy_elements(trainer -> current_pokemons, free);
    free(trainer);
}

void free_team_manager(){
    list_destroy_and_destroy_elements(global_goal, (void (*)(void *)) free);
    list_destroy_and_destroy_elements(localized_trainers, (void (*)(void *)) free_trainer);
}