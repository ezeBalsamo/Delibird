#include <catch_pokemon_operation_parser.h>
#include <stdlib.h>
#include "../../Utils/include/operations_information.h"

t_pokemon_operation_parser* catch_pokemon_parser;

bool catch_pokemon_can_handle(char* operation_name){
    return queue_code_of(operation_name) == CATCH_POKEMON;
}

void* catch_pokemon_parse_function(char** arguments){
    t_catch_pokemon* catch_pokemon = malloc(sizeof(t_catch_pokemon));
    catch_pokemon -> pokemon_name = arguments[0];
    catch_pokemon -> pos_x = atoi(arguments[1]);
    catch_pokemon -> pos_y = atoi(arguments[2]);
    catch_pokemon -> message_id = arguments[3] == NULL?-1:(atoi(arguments[3]));

    return catch_pokemon;
}

void initialize_catch_pokemon_operation_parser(){
    catch_pokemon_parser = malloc(sizeof(t_pokemon_operation_parser));
    catch_pokemon_parser -> can_handle_function = catch_pokemon_can_handle;
    catch_pokemon_parser -> parse_function = catch_pokemon_parse_function;
}

t_pokemon_operation_parser* catch_pokemon_operation_parser(){
    return catch_pokemon_parser;
}