#include "../include/team_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/common_structures.h"
#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>

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


void *get_team_actual_global_objective(){
    t_list *team_objective = list_create();

    //para cada trainer
    for (int i = 0;i<list_size(trainers);i++){
        //get si no hay nada devuelve null
        t_trainer *trainer = list_get(trainers,i);
        t_list *trainer_objective = get_actual_trainer_objective(trainer);
        if(list_is_empty(trainer_objective)){
            //romper porque en teoria estamos en el rango de trainers
            //log_errorful_message();
            exit(EXIT_FAILURE);
        }
        list_add(team_objective,trainer_objective);
    }

    //TODO: 1. aplanar lista, 2. buscar repetidos y "combinarlos",  3. devolver lista final :)
    //return team_objective;
}

//basicamente, por cada pokemon que "quiere",
// hacer 1 objetito con su nombre y la cantidad de veces que lo quiere
t_list *get_trainer_objective(t_trainer *trainer){
    //creo lista de objetivos. esto es lo que devuelvo al final
    t_list *trainer_objective = list_create();

    t_list *trainer_desired_pokemons = trainer->desired_pokemons;

    for (int i = 0; i<list_size(trainer_desired_pokemons);i++) {
        //get pokemon name
        char *pokemon_name = list_get(trainer_desired_pokemons, i);
        if (pokemon_name==NULL){
            //log_errorful_message();
            exit(EXIT_FAILURE);
        }
        //defino funcion para encontrar este pokemon en particular
        bool pokemon_is_equal_pokemon(void *pokemon_objective){
            return string_equals_ignore_case(((t_pokemon_goal*) pokemon_objective) -> pokemon_name, pokemon_name);
        }
        //trato de encontrar si ya existe el objetito, si no está (NULL) lo creo
        t_pokemon_goal *pokemon_objective = list_find(trainer_desired_pokemons, pokemon_is_equal_pokemon);
        if (pokemon_objective == NULL) {
            pokemon_objective->pokemon_name = pokemon_name;
            pokemon_objective->quantity = 1;
            list_add(trainer_objective,pokemon_objective);
        } else {
            int32_t amount = pokemon_objective->quantity;
            pokemon_objective->quantity = amount++;
            list_replace(trainer_objective,i,pokemon_objective);
        }
    }

    list_destroy(trainer_desired_pokemons);
    return trainer_objective;
}

t_list *get_actual_trainer_objective(t_trainer *trainer){
    t_list *trainer_objective = get_trainer_objective(trainer);
    t_list *trainer_current_pokemons = trainer->current_pokemons;
    //para cada pokemon que el trainer tiene, se lo voy a restar a sus objetivos
    //puede pasar de querer restar algo que no tengo, por lo que debo crear un nuevo objetito
    for(int i = 0;i < list_size(trainer_current_pokemons);i++){
        char *pokemon_name = list_get(trainer_current_pokemons,i);

        bool pokemon_is_equal_pokemon(void *pokemon_objective){
            return string_equals_ignore_case(((t_pokemon_goal*) pokemon_objective) -> pokemon_name, pokemon_name);
        }

        t_pokemon_goal *pokemon_objective = list_find(trainer_objective, pokemon_is_equal_pokemon);
        if (pokemon_objective == NULL) {
            pokemon_objective->pokemon_name = pokemon_name;
            pokemon_objective->quantity = -1;
            list_add(trainer_objective,pokemon_objective);
        } else {
            int32_t amount = pokemon_objective->quantity;
            pokemon_objective->quantity = amount--;
            list_replace(trainer_objective,i,pokemon_objective);
        }
    }
    list_destroy(trainer_current_pokemons);
    return trainer_objective;
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