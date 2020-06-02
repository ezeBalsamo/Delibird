#include <appeared_pokemon_operation_parser.h>
#include <stdlib.h>
#include "../../Utils/include/serializable_objects.h"

t_pokemon_operation_parser* appeared_pokemon_parser;

bool appeared_pokemon_can_handle(uint32_t operation_code){
    return operation_code == APPEARED_POKEMON;
}

bool should_build_correlative_identified_message_according_to(char** arguments){
    return arguments[4] != NULL;
}

t_identified_message* correlative_identified_message_built_with(t_identified_message* identified_message, char** arguments){
    t_request* correlative_request = safe_malloc(sizeof(t_request));
    correlative_request -> operation = IDENTIFIED_MESSAGE;
    correlative_request -> structure = (void*) identified_message;
    correlative_request -> sanitizer_function = (void (*)(void *)) free_identified_message;

    t_identified_message* correlative_identified_message = malloc(sizeof(t_identified_message));
    correlative_identified_message -> message_id = atoi(arguments[4]);
    correlative_identified_message -> request = correlative_request;

    return correlative_identified_message;
}

void* appeared_pokemon_parse_function(char** arguments){
    t_appeared_pokemon* appeared_pokemon = safe_malloc(sizeof(t_new_pokemon));
    appeared_pokemon -> pokemon_name = arguments[0];
    appeared_pokemon -> pos_x = atoi(arguments[1]);
    appeared_pokemon -> pos_y = atoi(arguments[2]);

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = APPEARED_POKEMON;
    request -> structure = (void*) appeared_pokemon;
    request -> sanitizer_function = free;

    t_identified_message* identified_message = malloc(sizeof(t_identified_message));
    identified_message -> message_id = atoi(arguments[3]);
    identified_message -> request = request;

    if(should_build_correlative_identified_message_according_to(arguments)){
        return correlative_identified_message_built_with(identified_message, arguments);
    }

    return identified_message;
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