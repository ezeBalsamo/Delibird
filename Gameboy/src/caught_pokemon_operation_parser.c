#include <caught_pokemon_operation_parser.h>
#include <stdlib.h>
#include "../../Utils/include/operations_information.h"

t_pokemon_operation_parser* caught_pokemon_parser;

bool caught_pokemon_can_handle(char* operation_name){
    return queue_code_of(operation_name) == CAUGHT_POKEMON;
}

void* caught_pokemon_parse_function(char** arguments){
    t_caught_pokemon* caught_pokemon = malloc(sizeof(t_caught_pokemon));
    caught_pokemon -> message_id = atoi(arguments[0]);
    caught_pokemon -> caught_status = atoi(arguments[1]);

    return caught_pokemon;
}

void initialize_caught_pokemon_operation_parser(){
    caught_pokemon_parser = malloc(sizeof(t_pokemon_operation_parser));
    caught_pokemon_parser -> can_handle_function = caught_pokemon_can_handle;
    caught_pokemon_parser -> parse_function = caught_pokemon_parse_function;
}

t_pokemon_operation_parser* caught_pokemon_operation_parser(){
    return caught_pokemon_parser;
}