#include <commons/collections/list.h>
#include <commons/string.h>
#include "../include/entry_point_validator.h"
#include "../include/entry_point_logs_manager.h"
#include "../include/role_mode_strategy.h"
#include "../../Utils/include/processes_information.h"
#include <stdio.h>

char** gameboy_arguments;
int gameboy_arguments_amount;
t_role_mode* role_mode;

void initialize_entry_point_validator(int arguments_amount, char** arguments){

    if (arguments_amount < 3) {
        incorrect_arguments_amount_error();
    }

    gameboy_arguments = arguments;
    gameboy_arguments_amount = arguments_amount;
    initialize_processes_information();
    role_mode = role_mode_for(gameboy_arguments[1]);
}

bool is_valid_operation(void* operation_information){
    return (*(role_mode -> is_valid_operation_function)) ((t_operation_information*) operation_information);
}

bool is_valid_publisher_operation(t_operation_information* operation_information){
    return string_equals_ignore_case(operation_information -> name, gameboy_arguments[2]);
}

bool is_valid_subscriber_operation(t_operation_information* operation_information){
    //TODO: agregar cuando se hayan hecho los cambios de processes_information y operation_information
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
    }

    int arguments_amount_difference = operation_information_found -> max_arguments_amount - gameboy_arguments_amount;

    if(arguments_amount_difference < 0 || arguments_amount_difference > 1){
        incorrect_arguments_amount_error();
    }

    return operation_information_found;
}

char** operation_arguments(){
    return &gameboy_arguments[3];
}

void free_entry_point_validator(){
    free_processes_information();
}