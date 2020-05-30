#include <new_pokemon_operation_parser.h>
#include <stdlib.h>
#include "../../Utils/include/serializable_objects.h"

t_pokemon_operation_parser* new_pokemon_parser;

bool new_pokemon_can_handle(uint32_t operation_code){
    return operation_code == NEW_POKEMON;
}

void* new_pokemon_parse_function(char** arguments){
    t_new_pokemon* new_pokemon = malloc(sizeof(t_new_pokemon));
    new_pokemon -> pokemon_name = arguments[0];
    new_pokemon -> pos_x = atoi(arguments[1]);
    new_pokemon -> pos_y = atoi(arguments[2]);
    new_pokemon -> quantity = atoi(arguments[3]);

    if(new_pokemon_parser -> should_build_identified_message){
        t_request* request = malloc(sizeof(t_request));
        request -> operation = NEW_POKEMON;
        request -> structure = (void*) new_pokemon;
        request -> sanitizer_function = free;

        t_identified_message* identified_message = malloc(sizeof(t_identified_message));
        identified_message -> message_id = atoi(arguments[4]);
        identified_message -> request = request;

        return identified_message;
    }

    return new_pokemon;
}

void initialize_new_pokemon_operation_parser_with(bool should_build_identified_message){
    new_pokemon_parser = malloc(sizeof(t_pokemon_operation_parser));
    new_pokemon_parser -> can_handle_function = new_pokemon_can_handle;
    new_pokemon_parser -> parse_function = new_pokemon_parse_function;
    new_pokemon_parser -> should_build_identified_message = should_build_identified_message;
}

t_pokemon_operation_parser* new_pokemon_operation_parser(){
    return new_pokemon_parser;
}