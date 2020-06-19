#include <caught_pokemon_operation_parser.h>
#include <stdlib.h>
#include <commons/string.h>
#include <entry_point_logs_manager.h>
#include "../../Utils/include/garbage_collector.h"

t_pokemon_operation_parser* caught_pokemon_parser;

bool caught_pokemon_can_handle(uint32_t operation_code){
    return operation_code == CAUGHT_POKEMON;
}

void validate_caught_status(char* caught_status){
    if(!string_equals_ignore_case(caught_status, "OK") &&
       !string_equals_ignore_case(caught_status, "FAIL")){
        log_invalid_caught_status_error(caught_status);
        free_system();
    }
}

void* caught_pokemon_parse_function(char** arguments){
    t_caught_pokemon* caught_pokemon = safe_malloc(sizeof(t_caught_pokemon));
    char* caught_status = arguments[1];

    validate_caught_status(caught_status);

    if(string_equals_ignore_case(caught_status, "OK")){
        caught_pokemon -> caught_status = 1;
    } else {
        caught_pokemon -> caught_status = 0;
    }

    if(caught_pokemon_parser -> should_build_identified_message){
        t_request* request = safe_malloc(sizeof(t_request));
        request -> operation = CAUGHT_POKEMON;
        request -> structure = (void*) caught_pokemon;
        request -> sanitizer_function = free;

        t_identified_message* identified_message = safe_malloc(sizeof(t_identified_message));
        identified_message -> message_id = atoi(arguments[0]);
        identified_message -> request = request;

        return identified_message;
    }

    return caught_pokemon;
}

void initialize_caught_pokemon_operation_parser_with(bool should_build_identified_message){
    caught_pokemon_parser = safe_malloc(sizeof(t_pokemon_operation_parser));
    caught_pokemon_parser -> can_handle_function = caught_pokemon_can_handle;
    caught_pokemon_parser -> parse_function = caught_pokemon_parse_function;
    caught_pokemon_parser -> should_build_identified_message = should_build_identified_message;
}

t_pokemon_operation_parser* caught_pokemon_operation_parser(){
    return caught_pokemon_parser;
}