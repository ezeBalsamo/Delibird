#include <get_pokemon_operation_parser.h>
#include <stdlib.h>
#include "../../Utils/include/operations_information.h"

t_pokemon_operation_parser* get_pokemon_parser;

bool get_pokemon_can_handle(char* operation_name){
    return queue_code_of(operation_name) == GET_POKEMON;
}

void* get_pokemon_parse_function(char** arguments){
    t_get_pokemon* get_pokemon = malloc(sizeof(t_get_pokemon));
    get_pokemon -> pokemon_name = arguments[0];
    get_pokemon -> message_id = arguments[1] == NULL?-1:(atoi(arguments[1]));

    return get_pokemon;
}

void initialize_get_pokemon_operation_parser(){
    get_pokemon_parser = malloc(sizeof(t_pokemon_operation_parser));
    get_pokemon_parser -> can_handle_function = get_pokemon_can_handle;
    get_pokemon_parser -> parse_function = get_pokemon_parse_function;
}

t_pokemon_operation_parser* get_pokemon_operation_parser(){
    return get_pokemon_parser;
}