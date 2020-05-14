#include "../include/team_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/common_structures.h"
#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>

t_list* trainers;
t_list* global_goal;

t_trainer* parsed_trainer_from(uint32_t sequential_number, char* positions, char* current_pokemons, char* desired_pokemons){
    t_trainer* trainer = malloc(sizeof(t_trainer));
    trainer -> sequential_number = sequential_number;

    char** splitted_positions = string_split(positions, "|");
    trainer -> pos_x = atoi(splitted_positions[0]);
    trainer -> pos_y = atoi(splitted_positions[1]);

    trainer -> current_pokemons = list_create();
    char** splitted_current_pokemons = string_split(current_pokemons, "|");
    for(int i = 0; splitted_current_pokemons[i] != NULL; i++){
        list_add(trainer -> current_pokemons, (void*) splitted_current_pokemons[i]);
    }

    trainer -> desired_pokemons = list_create();
    char** splitted_desired_pokemons = string_split(desired_pokemons, "|");
    for(int i = 0; splitted_desired_pokemons[i] != NULL; i++){
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

    for(uint32_t sequential_number = 0; positions[sequential_number] != NULL; sequential_number++){
        t_trainer* trainer = parsed_trainer_from(sequential_number, positions[sequential_number],
                                                 current_pokemons[sequential_number], desired_pokemons[sequential_number]);
        list_add(trainers, (void*) trainer);
    }

    free_char_array(positions);
    free_char_array(current_pokemons);
    free_char_array(desired_pokemons);
}

void calculate_global_goal(){
    //TODO lógica de fran para calcular el objetivo global del equipo
}

void initialize_team_manager(){
    trainers = list_create();
    global_goal = list_create();

    parse_trainers();
    calculate_global_goal();
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