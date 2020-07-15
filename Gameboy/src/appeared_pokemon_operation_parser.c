#include <appeared_pokemon_operation_parser.h>
#include <stdlib.h>
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/common_structures.h"

t_pokemon_operation_parser* appeared_pokemon_parser;

bool appeared_pokemon_can_handle(uint32_t operation_code){
    return operation_code == APPEARED_POKEMON;
}

uint32_t default_message_id_when_missing(){
    return 1; // Los argumentos de TEAM no incluyen id original ni id correlativo
}

uint32_t message_id_according_to(char** arguments){
    return arguments[3] == NULL ? default_message_id_when_missing() : atoi(arguments[3]);
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
    correlative_identified_message -> message_id = default_message_id_when_missing();
    correlative_identified_message -> request = correlative_request;

    return correlative_identified_message;
}

t_identified_message* identified_message_from(t_request* request, char** arguments){

    uint32_t message_id = message_id_according_to(arguments);

    t_identified_message* identified_message = malloc(sizeof(t_identified_message));
    identified_message -> message_id = message_id;
    identified_message -> request = request;

    if(should_build_correlative_identified_message_according_to(arguments)){
        return correlative_identified_message_built_with(identified_message, arguments);
    }

    return identified_message;
}

void* appeared_pokemon_parse_function(char** arguments){
    t_appeared_pokemon* appeared_pokemon = safe_malloc(sizeof(t_appeared_pokemon));
    appeared_pokemon -> pokemon_name = arguments[0];
    appeared_pokemon -> pos_x = atoi(arguments[1]);
    appeared_pokemon -> pos_y = atoi(arguments[2]);

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = APPEARED_POKEMON;
    request -> structure = appeared_pokemon;
    request -> sanitizer_function = free;

    return identified_message_from(request, arguments);
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