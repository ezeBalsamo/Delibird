#include "../include/entry_point_processes_information.h"
#include "../include/entry_point_logs_manager.h"
#include "../../Utils/include/operations_information.h"
#include <commons/string.h>
#include <stdlib.h>

t_list* processes_information;

t_list* team_operation_information(){
    t_list* operations = list_create();
    list_add(operations, appeared_pokemon_information());
    return operations;
}

t_list* broker_operation_information(){
    t_list* operations = list_create();
    list_add(operations, new_pokemon_information());
    list_add(operations, appeared_pokemon_information());
    list_add(operations, catch_pokemon_information());
    list_add(operations, caught_pokemon_information());
    list_add(operations, get_pokemon_information());
    return operations;
}

t_list* gamecard_operation_information(){
    t_list* operations = list_create();
    list_add(operations, new_pokemon_information());
    list_add(operations, catch_pokemon_information());
    list_add(operations, get_pokemon_information());
    return operations;
}

t_list* suscriptor_operation_information(){
    t_list* operations = list_create();
    list_add(operations, subscribe_me_information());
    return operations;
}

t_process_information* team_process_information(){
    t_process_information* process_information = malloc(sizeof(t_process_information));
    process_information -> code = TEAM;
    process_information -> name = "TEAM";
    process_information -> operations = team_operation_information();

    return process_information;
}

t_process_information* broker_process_information(){
    t_process_information* process_information = malloc(sizeof(t_process_information));
    process_information -> code = BROKER;
    process_information -> name = "BROKER";
    process_information -> operations = broker_operation_information();

    return process_information;
}

t_process_information* gamecard_process_information(){
    t_process_information* process_information = malloc(sizeof(t_process_information));
    process_information -> code = GAMECARD;
    process_information -> name = "GAMECARD";
    process_information -> operations = gamecard_operation_information();

    return process_information;
}

t_process_information* suscriptor_process_information(){
    t_process_information* process_information = malloc(sizeof(t_process_information));
    process_information -> code = SUSCRIPTOR;
    process_information -> name = "SUSCRIPTOR";
    process_information -> operations = suscriptor_operation_information();

    return process_information;
}

void initialize_processes_information(){

    processes_information = list_create();
    list_add(processes_information, (void*) team_process_information());
    list_add(processes_information, (void*) broker_process_information());
    list_add(processes_information, (void*) gamecard_process_information());
    list_add(processes_information, (void*) suscriptor_process_information());
}

t_process_information* process_information_named(char* process_name){

    bool _is_valid_process(void* process_information){
        return string_equals_ignore_case(((t_process_information*) process_information) -> name, process_name);
    }
    return list_find(processes_information, _is_valid_process);
}

uint32_t process_information_code_of(char* process_name){
    t_process_information* process_information = process_information_named(process_name);

    if(process_information == NULL){
        unknown_process_error_for(process_name);
    }

    return process_information -> code;
}

void free_processes_information(){
    list_destroy_and_destroy_elements(processes_information, (void (*)(void *)) free_process_information);
}