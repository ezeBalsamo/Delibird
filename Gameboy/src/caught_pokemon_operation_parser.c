#include <caught_pokemon_operation_parser.h>
#include <stdlib.h>
#include "../../Utils/include/serializable_objects.h"

t_pokemon_operation_parser* caught_pokemon_parser;

bool caught_pokemon_can_handle(uint32_t operation_code){
    return operation_code == CAUGHT_POKEMON;
}

void* caught_pokemon_parse_function(char** arguments){
    t_caught_pokemon* caught_pokemon = malloc(sizeof(t_caught_pokemon));
    caught_pokemon -> caught_status = atoi(arguments[1]);

    if(caught_pokemon_parser -> should_build_identified_message){
        t_request* request = malloc(sizeof(t_request));
        request -> operation = CAUGHT_POKEMON;
        request -> structure = (void*) caught_pokemon;

        t_identified_message* identified_message = malloc(sizeof(t_identified_message));
        identified_message -> message_id = atoi(arguments[0]);
        identified_message -> request = request;

        return identified_message;
    }

    return caught_pokemon;
}

void initialize_caught_pokemon_operation_parser_with(bool should_build_identified_message){
    caught_pokemon_parser = malloc(sizeof(t_pokemon_operation_parser));
    caught_pokemon_parser -> can_handle_function = caught_pokemon_can_handle;
    caught_pokemon_parser -> parse_function = caught_pokemon_parse_function;
    caught_pokemon_parser -> should_build_identified_message = should_build_identified_message;
}

t_pokemon_operation_parser* caught_pokemon_operation_parser(){
    return caught_pokemon_parser;
}