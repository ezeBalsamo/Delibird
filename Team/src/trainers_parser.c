#include <team_manager.h>
#include "trainers_parser.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/common_structures.h"
#include <commons/string.h>
#include <stdlib.h>

bool can_continue_parsing_current_pokemons = true;

void parsed_localized_object_positions(t_localizable_object* localizable_trainer, char* positions){

    char** splitted_positions = string_split(positions, "|");
    localizable_trainer -> pos_x = atoi(splitted_positions[0]);
    localizable_trainer -> pos_y = atoi(splitted_positions[1]);

    free_char_array(splitted_positions);
}

void parse_current_pokemons_for(t_trainer* trainer, char* current_pokemons){

    if(can_continue_parsing_current_pokemons){
        if(current_pokemons == NULL){

            can_continue_parsing_current_pokemons = false;
        }else{

            char** splitted_current_pokemons = string_split(current_pokemons, "|");

            for(int i = 0; splitted_current_pokemons[i]; i++){
                list_add(trainer -> current_pokemons, (void*) splitted_current_pokemons[i]);
            }

            free(splitted_current_pokemons);
        }
    }
}

void parse_required_pokemons_for(t_trainer* trainer, char* required_pokemons){

    char** splitted_required_pokemons = string_split(required_pokemons, "|");

    for(int i = 0; splitted_required_pokemons[i]; i++){
        list_add(trainer -> required_pokemons, (void*) splitted_required_pokemons[i]);
    }

    free(splitted_required_pokemons);
}

t_trainer* new_trainer_with_sequential_number(int sequential_number){

    t_trainer* trainer = safe_malloc(sizeof(t_trainer));
    trainer -> sequential_number = sequential_number;
    trainer -> current_pokemons = list_create();
    trainer -> required_pokemons = list_create();

    return trainer;
}

t_list* parsed_trainers(){

    t_list* trainers = list_create();

    char** positions = config_get_char_array_at("POSICIONES_ENTRENADORES");
    char** current_pokemons = config_get_char_array_at("POKEMON_ENTRENADORES");
    char** required_pokemons = config_get_char_array_at("OBJETIVOS_ENTRENADORES");

    for(int i = 0; positions[i]; i++){

        t_trainer* trainer = new_trainer_with_sequential_number(i + 1);

        if(*current_pokemons != NULL){
            parse_current_pokemons_for(trainer, current_pokemons[i]);
        }

        parse_required_pokemons_for(trainer, required_pokemons[i]);

        t_localizable_object* localizable_trainer = safe_malloc(sizeof(t_localizable_object));
        parsed_localized_object_positions(localizable_trainer, positions[i]);

        localizable_trainer -> type = TRAINER;
        localizable_trainer -> object = trainer;

        list_add(trainers, (void*) localizable_trainer);
    }

    free_char_array(positions);
    free_char_array(current_pokemons);
    free_char_array(required_pokemons);

    return trainers;
}