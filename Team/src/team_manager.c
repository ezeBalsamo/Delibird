#include <trainers_parser.h>
#include <goal_calculator.h>
#include <team_manager.h>
#include <map.h>
#include <stdlib.h>
#include <trainer_threads.h>
#include <commons/string.h>

t_list* trainers;
t_list* global_goal;

bool global_goal_contains(char* pokemon_name){
    t_list* actual_global_goal = team_global_goal_according_to(trainers);

    bool _is_equal_pokemon(void *pokemon_goal){
        return string_equals_ignore_case(((t_pokemon_goal*) pokemon_goal) -> pokemon_name, pokemon_name);
    }

    return list_any_satisfy(actual_global_goal, _is_equal_pokemon);
}

void* initialize_team_manager(){
    trainers = parsed_trainers();
    global_goal = team_global_goal_according_to(trainers);
    initialize_map();
    initialize_trainer_threads();

    return NULL;
}

bool are_equal_trainers(t_trainer* trainer, t_trainer* another_trainer){
    return trainer -> sequential_number == another_trainer -> sequential_number;
}

t_list* trainers_x_positions(){
    void* _x_position_of(void* trainer){
        return (void*) &(((t_trainer*) trainer) -> pos_x);
    }

    return list_map(trainers, _x_position_of);
}

t_list* trainers_y_positions(){
    void* _y_position_of(void* trainer){
        return (void*) &(((t_trainer*) trainer) -> pos_y);
    }

    return list_map(trainers, _y_position_of);
}

void with_trainers_do(void (*closure) (t_trainer*)){
    list_iterate(trainers, (void (*)(void *)) closure);
}

void with_global_goal_do(void (*closure) (t_pokemon_goal*)){
    list_iterate(global_goal, (void (*)(void *)) closure);
}

void free_trainer(t_trainer *trainer){
    list_destroy_and_destroy_elements(trainer -> desired_pokemons, free);
    list_destroy_and_destroy_elements(trainer -> current_pokemons, free);
    free(trainer);
}

void free_team_manager(){
    list_destroy_and_destroy_elements(global_goal, (void (*)(void *)) free);
    list_destroy_and_destroy_elements(trainers, (void (*)(void *)) free_trainer);
}