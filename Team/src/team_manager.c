#include "../include/team_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/common_structures.h"
#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>

t_list* trainers;

bool pokemon_objective_exists(t_objective objective, t_list *objectives);

t_trainer* parsed_trainer_from(char* positions, char* current_pokemons, char* desired_pokemons){
    t_trainer* trainer = malloc(sizeof(t_trainer));

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

void initialize_trainers(){
    char** positions = config_get_char_array_at("POSICIONES_ENTRENADORES");
    char** current_pokemons = config_get_char_array_at("POKEMON_ENTRENADORES");
    char** desired_pokemons = config_get_char_array_at("OBJETIVOS_ENTRENADORES");

    for(int i=0; positions[i] != NULL; i++){
        t_trainer* trainer = parsed_trainer_from(positions[i], current_pokemons[i], desired_pokemons[i]);
        list_add(trainers, (void*) trainer);
    }

    free_char_array(positions);
    free_char_array(current_pokemons);
    free_char_array(desired_pokemons);
}

void initialize_team_manager(){
    trainers = list_create();
    initialize_trainers();
}


void *get_team_actual_global_objective(){
    t_list *team_objective;

    //para cada trainer
    for (int i = 0;i<list_size(trainers);i++){
        //get si no hay nada devuelve null
        t_objective *trainer_objective = list_get(trainers,i);
        if(trainer_objective == NULL){
            //romper porque en teoria estamos en el rango de trainers
        }
        list_add(team_objective,trainer_objective);
    }

    return team_objective;
}

void *get_actual_trainer_objective(t_trainer *trainer){
    //creo lista de objetivos. esto es lo que devuelvo al final
    t_list *trainer_objectives = list_create();

    t_list *trainer_desired_pokemons = trainer->desired_pokemons;

    for (int i = 0; i<list_size(trainer_desired_pokemons);i++){
        //get pokemon name
        char* pokemon_name = list_get(trainer_desired_pokemons,i);
        //create objective

        //check if exists,then increase amount or add new objective
        for (int j=0;j<list_size(trainer_objectives);j++){

            t_objective *pokemon_objective = list_get(trainer_objectives,j);

            if (pokemon_objective != NULL && pokemon_name == pokemon_objective->pokemon_name){
                int  amount =pokemon_objective->amount_to_catch;
                pokemon_objective->amount_to_catch = amount++;
                list_replace(trainer_objectives,j,pokemon_objective);
                //since we found the pokemon on our objectives and updated it, break the loop
                break;
            }
            //if its the last iteration, and we didnt break yet
            //means that there was no objective associated to this pokemon
            //so we create it
            if (j==list_size(trainer_objectives)){
                pokemon_objective ->pokemon_name = pokemon_name;
                pokemon_objective->amount_to_catch= 1;
                list_add(trainer_objectives,pokemon_objective);
            }

        }
    }
    return trainer_objectives;
}
bool pokemon_exists(void* trainer_objs){
    return false;
}
