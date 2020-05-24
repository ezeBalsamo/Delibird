#include <pokemon_operation_parser.h>
#include <commons/collections/list.h>
#include <entry_point_logs_manager.h>
#include <stdlib.h>
#include <new_pokemon_operation_parser.h>
#include <appeared_pokemon_operation_parser.h>
#include <catch_pokemon_operation_parser.h>
#include <caught_pokemon_operation_parser.h>
#include <get_pokemon_operation_parser.h>

t_list* pokemon_operation_parsers;

void initialize_pokemon_operation_parser(){
    initialize_new_pokemon_operation_parser();
    initialize_appeared_pokemon_operation_parser();
    initialize_get_pokemon_operation_parser();
    initialize_catch_pokemon_operation_parser();
    initialize_caught_pokemon_operation_parser();

    pokemon_operation_parsers = list_create();
    list_add(pokemon_operation_parsers, (void*) new_pokemon_operation_parser());
    list_add(pokemon_operation_parsers, (void*) appeared_pokemon_operation_parser());
    list_add(pokemon_operation_parsers, (void*) get_pokemon_operation_parser());
    list_add(pokemon_operation_parsers, (void*) catch_pokemon_operation_parser());
    list_add(pokemon_operation_parsers, (void*) caught_pokemon_operation_parser());
}

void free_pokemon_operation_parser(){
    list_destroy_and_destroy_elements(pokemon_operation_parsers, free);
}

t_pokemon_operation_parser* pokemon_operation_parser_for(char* operation_name){

    initialize_pokemon_operation_parser();

    bool _can_handle(void* pokemon_operation_parser){
        t_pokemon_operation_parser* cast_parser = (t_pokemon_operation_parser*) pokemon_operation_parser;
        return (*(cast_parser -> can_handle_function)) (operation_name);
    }

    t_pokemon_operation_parser* parser_found = list_remove_by_condition(pokemon_operation_parsers, _can_handle);

    if(parser_found == NULL){
        log_no_parser_suitable_for_operation_error_for(operation_name);
    }

    free_pokemon_operation_parser();

    return parser_found;
}