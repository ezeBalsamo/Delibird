#include "../include/team_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/t_list_extension.h"
#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>

t_list* trainers;
t_list* global_goal;

t_list* requirements_of(t_trainer* trainer){
    return list_difference(trainer -> desired_pokemons, trainer -> current_pokemons,
                           (bool (*)(void *, void *)) string_equals_ignore_case);
}

t_trainer* parsed_trainer_from(uint32_t sequential_number, char* positions, char* current_pokemons, char* desired_pokemons){
    t_trainer* trainer = malloc(sizeof(t_trainer));
    trainer -> sequential_number = sequential_number;

    char** splitted_positions = string_split(positions, "|");
    trainer -> pos_x = atoi(splitted_positions[0]);
    trainer -> pos_y = atoi(splitted_positions[1]);

    trainer -> current_pokemons = list_create();
    char** splitted_current_pokemons = string_split(current_pokemons, "|");
    for(int i = 0; splitted_current_pokemons[i]; i++){
        list_add(trainer -> current_pokemons, (void*) splitted_current_pokemons[i]);
    }

    trainer -> desired_pokemons = list_create();
    char** splitted_desired_pokemons = string_split(desired_pokemons, "|");
    for(int i = 0; splitted_desired_pokemons[i]; i++){
        list_add(trainer -> desired_pokemons, (void*) splitted_desired_pokemons[i]);
    }

    free_char_array(splitted_positions);
    free(splitted_current_pokemons);
    free(splitted_desired_pokemons);

    return trainer;
}

void parse_trainers(){
    char** positions = config_get_char_array_at("POSICIONES_ENTRENADORES");
    char** current_pokemons = config_get_char_array_at("POKEMON_ENTRENADORES");
    char** desired_pokemons = config_get_char_array_at("OBJETIVOS_ENTRENADORES");

    for(uint32_t sequential_number = 0; positions[sequential_number]; sequential_number++){
        t_trainer* trainer = parsed_trainer_from(sequential_number, positions[sequential_number],
                                                 current_pokemons[sequential_number], desired_pokemons[sequential_number]);
        list_add(trainers, (void*) trainer);
    }

    free_char_array(positions);
    free_char_array(current_pokemons);
    free_char_array(desired_pokemons);
}

void initialize_team_manager(){
    trainers = list_create();
    global_goal = list_create();
    parse_trainers();
    calculate_global_goal();
}

t_list* goal_of(t_trainer* trainer){
    t_list* trainer_goal = list_create();
    t_list* trainer_required_pokemons = requirements_of(trainer);

    for (int i = 0; i <list_size(trainer_required_pokemons); i++) {

        char *pokemon_name = (char *) list_get(trainer_required_pokemons, i);

        bool _is_equal_pokemon(void *pokemon_objective){
            return string_equals_ignore_case(((t_pokemon_goal*) pokemon_objective) -> pokemon_name, pokemon_name);
        }

        t_pokemon_goal* pokemon_goal = list_find(trainer_goal, _is_equal_pokemon);

        if (pokemon_goal == NULL) {
            pokemon_goal = malloc(sizeof(t_pokemon_goal));
            pokemon_goal -> pokemon_name = pokemon_name;
            pokemon_goal -> quantity = 1;
            list_add(trainer_goal, (void*) pokemon_goal);
        } else {
            uint32_t amount = pokemon_goal -> quantity;
            pokemon_goal -> quantity = amount++;
        }
    }

    list_destroy(trainer_required_pokemons);
    return trainer_goal;
}

void unify_pokemon_goals(t_list* flattened_team_pokemon_goals) {

    for (int i = 0; i < list_size(flattened_team_pokemon_goals); i++) {

        t_pokemon_goal* trainer_pokemon_goals = list_get(flattened_team_pokemon_goals, i);

        bool _is_equal_pokemon(void* pokemon_goal){
            return string_equals_ignore_case(((t_pokemon_goal*) pokemon_goal) -> pokemon_name,
                                             trainer_pokemon_goals -> pokemon_name);
        }

        t_pokemon_goal* global_pokemon_goal = list_find(global_goal, _is_equal_pokemon);

        if (global_pokemon_goal == NULL) {
            global_pokemon_goal = malloc(sizeof(t_pokemon_goal));
            global_pokemon_goal -> pokemon_name = trainer_pokemon_goals -> pokemon_name;
            global_pokemon_goal -> quantity = trainer_pokemon_goals -> quantity;
            list_add(global_goal, (void *) global_pokemon_goal);
        } else {
            uint32_t amount_to_sum = global_pokemon_goal -> quantity;
            global_pokemon_goal -> quantity = amount_to_sum + trainer_pokemon_goals -> quantity;
        }
    }

}

void calculate_global_goal(){
    t_list* team_pokemon_goals = list_create();

    for (int i = 0; i < list_size(trainers); i++){
        t_trainer* trainer = list_get(trainers, i);
        t_list* trainer_pokemon_goals = goal_of(trainer);
        list_add(team_pokemon_goals, trainer_pokemon_goals);
    }

    t_list* flattened_team_pokemon_goals = list_flat(team_pokemon_goals);
    unify_pokemon_goals(flattened_team_pokemon_goals);

    list_of_lists_destroy_and_destroy_elements(team_pokemon_goals, (void (*)(void *)) free);
    list_destroy(flattened_team_pokemon_goals);
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
    list_destroy_and_destroy_elements(trainer->desired_pokemons,free);
    list_destroy_and_destroy_elements(trainer->current_pokemons,free);
    free(trainer);
}
void free_team_manager(){
    list_destroy_and_destroy_elements(global_goal, (void (*)(void *)) free);
    list_destroy_and_destroy_elements(trainers, (void (*)(void *)) free_trainer);
}
