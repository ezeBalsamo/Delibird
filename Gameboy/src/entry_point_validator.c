#include <commons/collections/list.h>
#include <commons/string.h>
#include "../include/entry_point_validator.h"
#include "../../Utils/include/processes_information.h"
#include <stdlib.h>
#include <stdio.h>

char** gameboy_arguments;
int gameboy_arguments_amount;

void unknown_process_error_for(char* process_name){
    printf("%s no está registrado como un proceso válido.\n", process_name);
    exit(EXIT_FAILURE);
}

void unknown_operation_error_for(char* process_name, char* operation_name){
    printf("%s no está registrada como una operación válida para %s.\n", operation_name, process_name);
    exit(EXIT_FAILURE);
}

void incorrect_arguments_amount_error(){
    printf("La cantidad de argumentos ingresada es incorrecta.\n");
    exit(EXIT_FAILURE);
}

void initialize_entry_point_validator(int arguments_amount, char** arguments){

    if (arguments_amount < 3) {
        incorrect_arguments_amount_error();
        exit(EXIT_FAILURE);
    }

    gameboy_arguments = arguments;
    gameboy_arguments_amount = arguments_amount;
    initialize_processes_information();
}

bool is_valid_operation(void* operation_information){
    return string_equals_ignore_case(((t_operation_information*) operation_information) -> name, gameboy_arguments[2]);
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

    if((operation_information_found -> max_arguments_amount - gameboy_arguments_amount) > 1){
        incorrect_arguments_amount_error();
    }

    free_process_information(process_information_found);

    return operation_information_found;
}

char** operation_arguments(){
    return &gameboy_arguments[3];
}

void free_entry_point_validator(){
    free_char_array(gameboy_arguments);
    free_processes_information();
}