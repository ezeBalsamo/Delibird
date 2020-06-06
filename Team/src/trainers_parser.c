#include <team_manager.h>
#include "trainers_parser.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/common_structures.h"
#include <commons/string.h>
#include <stdlib.h>

void parsed_localized_object_positions(t_localizable_object* localizable_trainer, char* positions){

    char** splitted_positions = string_split(positions, "|");
    localizable_trainer -> pos_x = atoi(splitted_positions[0]);
    localizable_trainer -> pos_y = atoi(splitted_positions[1]);

    free_char_array(splitted_positions);
}

t_trainer* parsed_trainer_from(uint32_t sequential_number, char* current_pokemons, char* desired_pokemons){

    t_trainer* trainer = safe_malloc(sizeof(t_trainer));
    trainer -> sequential_number = sequential_number;

    trainer -> current_pokemons = list_create();
    char** splitted_current_pokemons = string_split(current_pokemons, "|");
    for(int i = 0; splitted_current_pokemons[i]; i++){
        list_add(trainer -> current_pokemons, (void*) splitted_current_pokemons[i]);
    }

    trainer -> required_pokemons = list_create();
    char** splitted_desired_pokemons = string_split(desired_pokemons, "|");
    for(int i = 0; splitted_desired_pokemons[i]; i++){
        list_add(trainer -> required_pokemons, (void*) splitted_desired_pokemons[i]);
    }

    free(splitted_current_pokemons);
    free(splitted_desired_pokemons);

    return trainer;
}

t_list* parsed_trainers(){

    t_list* trainers = list_create();

    char** positions = config_get_char_array_at("POSICIONES_ENTRENADORES");
    char** current_pokemons = config_get_char_array_at("POKEMON_ENTRENADORES");
    char** desired_pokemons = config_get_char_array_at("OBJETIVOS_ENTRENADORES");

    for(uint32_t sequential_number = 0; positions[sequential_number]; sequential_number++){

        t_trainer* trainer = parsed_trainer_from(sequential_number,
                current_pokemons[sequential_number], desired_pokemons[sequential_number]);

        t_localizable_object* localizable_trainer = safe_malloc(sizeof(t_localizable_object));
        parsed_localized_object_positions(localizable_trainer, positions[sequential_number]);

        localizable_trainer -> object = trainer;

        list_add(trainers, (void*) localizable_trainer);
    }

    free_char_array(positions);
    free_char_array(current_pokemons);
    free_char_array(desired_pokemons);

    return trainers;
}