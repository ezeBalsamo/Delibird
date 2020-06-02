#include <appeared_pokemon_operation_parser.h>
#include <stdlib.h>
#include "../../Utils/include/serializable_objects.h"

t_pokemon_operation_parser* appeared_pokemon_parser;

bool appeared_pokemon_can_handle(uint32_t operation_code){
    return operation_code == APPEARED_POKEMON;
}

void* appeared_pokemon_parse_function(char** arguments){
    t_appeared_pokemon* appeared_pokemon = safe_malloc(sizeof(t_new_pokemon));
    appeared_pokemon -> pokemon_name = arguments[0];
    appeared_pokemon -> pos_x = atoi(arguments[1]);
    appeared_pokemon -> pos_y = atoi(arguments[2]);

    if (appeared_pokemon_parser -> should_build_identified_message){
        t_request* request = safe_malloc(sizeof(t_request));
        request -> operation = APPEARED_POKEMON;
        request -> structure = (void*) appeared_pokemon;
        request -> sanitizer_function = free;

        t_identified_message* identified_message = safe_malloc(sizeof(t_identified_message));
        identified_message -> message_id = atoi(arguments[3]);
        identified_message -> request = request;

        return identified_message;
    }

    return appeared_pokemon;
}

void initialize_appeared_pokemon_operation_parser_with(bool should_build_identified_message){
    appeared_pokemon_parser = safe_malloc(sizeof(t_pokemon_operation_parser));
    appeared_pokemon_parser -> can_handle_function = appeared_pokemon_can_handle;
    appeared_pokemon_parser -> parse_function = appeared_pokemon_parse_function;
    appeared_pokemon_parser -> should_build_identified_message = should_build_identified_message;
}

t_pokemon_operation_parser* appeared_pokemon_operation_parser(){
    return appeared_pokemon_parser;
}