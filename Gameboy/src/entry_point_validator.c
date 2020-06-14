#include <commons/collections/list.h>
#include <commons/string.h>
#include "../include/entry_point_validator.h"
#include "../include/entry_point_logs_manager.h"
#include "../include/role_mode_strategy.h"
#include <pokemon_operation_parser.h>
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include "../../Utils/include/garbage_collector.h"
#include <stdlib.h>

char** gameboy_arguments;
int gameboy_arguments_amount;
t_role_mode* role_mode;

void initialize_entry_point_validator(int arguments_amount, char** arguments){

    if (arguments_amount < 3) {
        incorrect_arguments_amount_error();
        free_system();
    }

    gameboy_arguments = arguments;
    gameboy_arguments_amount = arguments_amount;
    initialize_processes_information();
    uint32_t process_code = process_information_code_of(gameboy_arguments[1]);
    role_mode = role_mode_for(process_code);

    log_succesful_initialize_entry_point_validator();
}

bool is_valid_operation(void* operation_information){
    return (*(role_mode -> is_valid_operation_function)) ((t_operation_information*) operation_information);
}

bool is_valid_publisher_operation(t_operation_information* operation_information){
    return string_equals_ignore_case(operation_information -> name, gameboy_arguments[2]);
}

bool is_valid_subscriber_operation(t_operation_information* operation_information){
    (void) operation_information; // Se utiliza para suprimir warning debido a double-dispatch
    return exists_queue_named(gameboy_arguments[2]);
}

t_process_information* valid_chosen_process(){
    //OJO EL ./gameboy es el lugar 0.
    t_process_information* process_information_found = process_information_named(gameboy_arguments[1]);

    if(process_information_found == NULL){
        unknown_process_error_for(gameboy_arguments[1]);
    }
    return process_information_found;
}

t_operation_information* valid_chosen_operation(){

    t_process_information* process_information_found = valid_chosen_process();

    t_operation_information* operation_information_found =
            list_find(process_information_found -> operations, is_valid_operation);

    if(operation_information_found == NULL){
        unknown_operation_error_for(process_information_found -> name, gameboy_arguments[2]);
        free_system();
    }

    int arguments_amount_difference = operation_information_found -> arguments_amount - gameboy_arguments_amount;

    if(arguments_amount_difference != 0){
        incorrect_arguments_amount_error();
        free_system();
    }

    return operation_information_found;
}

uint32_t pokemon_operation_code(){
    t_operation_information* chosen_operation = valid_chosen_operation();

    return chosen_operation -> has_identified_message?IDENTIFIED_MESSAGE:chosen_operation -> serializable_object -> code;
}

void* pokemon_operation_structure(){
    return (role_mode -> pokemon_operation_structure_function)();
}

void* pokemon_operation_sanitizer_function(){
    if(valid_chosen_operation() -> has_identified_message){
       return free_identified_message;
    }
    return free;
}

void* publisher_pokemon_operation_structure(){

    t_operation_information* chosen_operation = valid_chosen_operation();
    t_pokemon_operation_parser* parser =
            pokemon_operation_parser_for(
                        chosen_operation -> serializable_object -> code,
                        chosen_operation -> has_identified_message);

    void* structure = (*(parser -> parse_function)) (&gameboy_arguments[3]);

    free(parser);
    return structure;
}

t_list* gameboy_arguments_to_hash(){
    t_list* gameboy_arguments_to_hash = list_create();

    for(int i = 1; i < gameboy_arguments_amount; i++){
        char* argument = gameboy_arguments[i];
        list_add(gameboy_arguments_to_hash, string_duplicate(argument));
    }

    return gameboy_arguments_to_hash;
}

void* subscriber_pokemon_operation_structure(){

    t_list* arguments_to_hash = gameboy_arguments_to_hash();

    char* queue_name = gameboy_arguments[2];
    char* process_description = process_description_for("GAMEBOY", arguments_to_hash);

    list_destroy_and_destroy_elements(arguments_to_hash, free);

    // Se toma la decisión de considerarlo basura ya que cambiar pokemon_operation_sanitizer_function
    // es más complejo para lo que representa la solución al problema
    consider_as_garbage(process_description, free);

    t_subscribe_me* subscribe_me = safe_malloc(sizeof(t_subscribe_me));
    subscribe_me -> operation_queue = queue_code_of(queue_name);
    subscribe_me -> process_description = process_description;
    return (void*) subscribe_me;
}

char* valid_process_name_for_connection(){
    return role_mode -> process_name_for_connection;
}

bool is_subscriber_mode(){
    return role_mode -> is_subscriber_mode;
}

int suscriber_mode_time_in_seconds(){
    if(!is_subscriber_mode()){
        log_operation_only_allowed_for_susbcriber_mode_error();
        free_system();
    }

    return atoi(gameboy_arguments[3]);
}

void free_entry_point_validator(){
    free_processes_information();
    free_serializable_objects();
    free_role_mode_strategy();
}