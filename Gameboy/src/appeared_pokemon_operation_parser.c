#include <appeared_pokemon_operation_parser.h>
#include <stdlib.h>
#include "../../Utils/include/operations_information.h"

t_pokemon_operation_parser* appeared_pokemon_parser;

bool appeared_pokemon_can_handle(char* operation_name){
    return queue_code_of(operation_name) == APPEARED_POKEMON;
}

void* appeared_pokemon_parse_function(char** arguments){
    t_appeared_pokemon* appeared_pokemon = malloc(sizeof(t_new_pokemon));
    appeared_pokemon -> pokemon_name = arguments[0];
    appeared_pokemon -> pos_x = atoi(arguments[1]);
    appeared_pokemon -> pos_y = atoi(arguments[2]);
    appeared_pokemon -> message_id = arguments[3] == NULL?-1:(atoi(arguments[3]));

    return appeared_pokemon;
}

void initialize_appeared_pokemon_operation_parser(){
    appeared_pokemon_parser = malloc(sizeof(t_pokemon_operation_parser));
    appeared_pokemon_parser -> can_handle_function = appeared_pokemon_can_handle;
    appeared_pokemon_parser -> parse_function = appeared_pokemon_parse_function;
}

t_pokemon_operation_parser* appeared_pokemon_operation_parser(){
    return appeared_pokemon_parser;
}