#include <pokemon_operation_parser.h>
#include <commons/collections/list.h>
#include <entry_point_logs_manager.h>
#include <new_pokemon_operation_parser.h>
#include <appeared_pokemon_operation_parser.h>
#include <catch_pokemon_operation_parser.h>
#include <caught_pokemon_operation_parser.h>
#include <get_pokemon_operation_parser.h>
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include "../../Utils/include/garbage_collector.h"
#include <stdlib.h>

t_list* pokemon_operation_parsers;

void initialize_pokemon_operation_parser_with(bool should_build_identified_message){

    initialize_new_pokemon_operation_parser_with(should_build_identified_message);
    initialize_appeared_pokemon_operation_parser_with(should_build_identified_message);
    initialize_get_pokemon_operation_parser_with(should_build_identified_message);
    initialize_catch_pokemon_operation_parser_with(should_build_identified_message);
    initialize_caught_pokemon_operation_parser_with(should_build_identified_message);

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

t_pokemon_operation_parser* pokemon_operation_parser_for(uint32_t operation_code, bool should_build_identified_message){

    initialize_pokemon_operation_parser_with(should_build_identified_message);

    bool _can_handle(void* pokemon_operation_parser){
        t_pokemon_operation_parser* cast_parser = (t_pokemon_operation_parser*) pokemon_operation_parser;
        return (*(cast_parser -> can_handle_function)) (operation_code);
    }

    t_pokemon_operation_parser* parser_found = list_remove_by_condition(pokemon_operation_parsers, _can_handle);

    if(parser_found == NULL){
        log_no_parser_suitable_for_operation_error_for(queue_name_of(operation_code));
        free_system();
    }

    free_pokemon_operation_parser();

    return parser_found;
}