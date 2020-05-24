#include <new_pokemon_operation_parser.h>
#include <stdlib.h>
#include "../../Utils/include/operations_information.h"

t_pokemon_operation_parser* new_pokemon_parser;

bool new_pokemon_can_handle(char* operation_name){
    return queue_code_of(operation_name) == NEW_POKEMON;
}

void* new_pokemon_parse_function(char** arguments){
    t_new_pokemon* new_pokemon = malloc(sizeof(t_new_pokemon));
    new_pokemon -> pokemon_name = arguments[0];
    new_pokemon -> pos_x = atoi(arguments[1]);
    new_pokemon -> pos_y = atoi(arguments[2]);
    new_pokemon -> quantity = atoi(arguments[3]);
    new_pokemon -> message_id = arguments[4] == NULL?-1:(atoi(arguments[4]));

    return new_pokemon;
}

void initialize_new_pokemon_operation_parser(){
    new_pokemon_parser = malloc(sizeof(t_pokemon_operation_parser));
    new_pokemon_parser -> can_handle_function = new_pokemon_can_handle;
    new_pokemon_parser -> parse_function = new_pokemon_parse_function;
}

t_pokemon_operation_parser* new_pokemon_operation_parser(){
    return new_pokemon_parser;
}