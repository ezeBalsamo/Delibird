#include "../include/team_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/t_list_extension.h"
#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>

t_list* trainers;
t_list* global_goal;

t_list* requirements_from_trainer(t_trainer* trainer){
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
    t_list *team_objective = list_create();

    for (int i = 0;i<list_size(trainers);i++){
        //get si no hay nada devuelve null
        t_trainer *trainer = list_get(trainers,i);

        t_list *trainer_objective = get_trainer_objective(trainer,team_objective);

        list_add(team_objective,trainer_objective);
    }
    t_list *team_objective_flattened = list_flat(team_objective);

    //logica para unificar objetivos con pokemones repetidos...
    unify_pokemon_goals(team_objective_flattened);

}


void initialize_team_manager(){
    trainers = list_create();
    global_goal = list_create();
    parse_trainers();
    calculate_global_goal();
}


//crea objetitos por pokemons "requeridos"
t_list *get_trainer_objective(t_trainer *trainer, t_list *team_objective){

    //creo lista de objetivos. esto es lo que devuelvo al final
    t_list *trainer_objective = list_create();

    //required = desired - current (A-B resta de conjuntos)
    t_list *trainer_required_pokemons = list_create();
    trainer_required_pokemons = requirements_from_trainer(trainer);

    for (int i = 0; i<list_size(trainer_required_pokemons);i++) {
        //get pokemon name
        char *pokemon_name = (char *)list_get(trainer_required_pokemons, i);

        //defino funcion para encontrar este pokemon en particular
        bool is_equal_pokemon(void *pokemon_objective){
            return string_equals_ignore_case(((t_pokemon_goal*) pokemon_objective) -> pokemon_name, pokemon_name);
        }
        //trato de encontrar si ya existe el objetito, si no está (NULL) lo creo
        t_pokemon_goal *pokemon_objective = list_find(trainer_objective, is_equal_pokemon);
        if (pokemon_objective == NULL) {
            pokemon_objective = malloc(sizeof(t_pokemon_goal));
            pokemon_objective->pokemon_name = pokemon_name;
            pokemon_objective->quantity = 1;
            list_add(trainer_objective,(void*) pokemon_objective);
        } else {
            uint32_t amount = pokemon_objective->quantity;
            pokemon_objective->quantity = amount++;
            list_replace(trainer_objective,i,(void*) pokemon_objective);
        }
    }

    list_destroy(trainer_required_pokemons);
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

void unify_pokemon_goals(t_list* pokemon_goals_flattened) {

    for (int j = 0; j < list_size(pokemon_goals_flattened); j++) {

        t_pokemon_goal *trainer_pok_goal = list_get(pokemon_goals_flattened, j);
        bool is_equal_pokemon(void *pokemon_objective) {
            return string_equals_ignore_case(((t_pokemon_goal *) pokemon_objective)->pokemon_name,
                                             trainer_pok_goal->pokemon_name);
        }
        //busco si en mis goal existe el pokemon el pokemon que trato de "combinar"
        t_pokemon_goal *global_pok_goal = list_find(global_goal, is_equal_pokemon);
        if (global_pok_goal == NULL) {
            global_pok_goal = malloc(sizeof(t_pokemon_goal));
            global_pok_goal->pokemon_name = trainer_pok_goal->pokemon_name;
            global_pok_goal->quantity = trainer_pok_goal->quantity;
            list_add(global_goal, (void *) global_pok_goal);
        } else {
            uint32_t sum = global_pok_goal->quantity;
            global_pok_goal->quantity = sum + trainer_pok_goal->quantity;
            list_replace(global_goal, j, (void *) global_pok_goal);
        }
    }

}
