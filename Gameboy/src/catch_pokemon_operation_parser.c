#include <catch_pokemon_operation_parser.h>
#include <stdlib.h>
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/common_structures.h"

t_pokemon_operation_parser* catch_pokemon_parser;

bool catch_pokemon_can_handle(uint32_t operation_code){
    return operation_code == CATCH_POKEMON;
}

void* catch_pokemon_parse_function(char** arguments){
    t_catch_pokemon* catch_pokemon = safe_malloc(sizeof(t_catch_pokemon));
    catch_pokemon -> pokemon_name = arguments[0];
    catch_pokemon -> pos_x = atoi(arguments[1]);
    catch_pokemon -> pos_y = atoi(arguments[2]);

    if (catch_pokemon_parser -> should_build_identified_message){
        t_request* request = safe_malloc(sizeof(t_request));
        request -> operation = CATCH_POKEMON;
        request -> structure = (void*) catch_pokemon;
        request -> sanitizer_function = free;

        t_identified_message* identified_message = safe_malloc(sizeof(t_identified_message));
        identified_message -> message_id = atoi(arguments[3]);
        identified_message -> request = request;

        return identified_message;
    }

    return catch_pokemon;
}

void initialize_catch_pokemon_operation_parser_with(bool should_build_identified_message){
    catch_pokemon_parser = safe_malloc(sizeof(t_pokemon_operation_parser));
    catch_pokemon_parser -> can_handle_function = catch_pokemon_can_handle;
    catch_pokemon_parser -> parse_function = catch_pokemon_parse_function;
    catch_pokemon_parser -> should_build_identified_message = should_build_identified_message;
}

t_pokemon_operation_parser* catch_pokemon_operation_parser(){
    return catch_pokemon_parser;
}