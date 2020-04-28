#include <commons/collections/list.h>
#include <commons/string.h>
#include "../include/processes_information.h"
#include "../include/common_structures.h"
#include "../include/operation_serialization.h"
#include "../include/operation_deserialization.h"
#include <stdlib.h>

t_list* processes_information;

t_operation_information* new_pokemon_information(){
    t_operation_information* operation_information = malloc(sizeof(t_operation_information));
    operation_information -> code = NEW_POKEMON;
    operation_information -> name = "NEW_POKEMON";
    operation_information -> max_arguments_amount = 8;
    operation_information -> serialize_function = serialize_new_pokemon;
    operation_information -> deserialize_function = deserialize_new_pokemon;

    return operation_information;
}

t_operation_information* appeared_pokemon_information(){
    t_operation_information* operation_information = malloc(sizeof(t_operation_information));
    operation_information -> code = APPEARED_POKEMON;
    operation_information -> name = "APPEARED_POKEMON";
    operation_information -> max_arguments_amount = 7;
    operation_information -> serialize_function = serialize_appeared_pokemon;
    operation_information -> deserialize_function = deserialize_appeared_pokemon;

    return operation_information;
}

t_operation_information* catch_pokemon_information(){
    t_operation_information* operation_information = malloc(sizeof(t_operation_information));
    operation_information -> code = CATCH_POKEMON;
    operation_information -> name = "CATCH_POKEMON";
    operation_information -> max_arguments_amount = 7;
    operation_information -> serialize_function = serialize_catch_pokemon;
    operation_information -> deserialize_function = deserialize_catch_pokemon;

    return operation_information;
}

t_operation_information* caught_pokemon_information(){
    t_operation_information* operation_information = malloc(sizeof(t_operation_information));
    operation_information -> code = CAUGHT_POKEMON;
    operation_information -> name = "CAUGHT_POKEMON";
    operation_information -> max_arguments_amount = 5;
    operation_information -> serialize_function = serialize_caught_pokemon;
    operation_information -> deserialize_function = deserialize_caught_pokemon;

    return operation_information;
}

t_operation_information* get_pokemon_information(){
    t_operation_information* operation_information = malloc(sizeof(t_operation_information));
    operation_information -> code = GET_POKEMON;
    operation_information -> name = "GET_POKEMON";
    operation_information -> max_arguments_amount = 4;
    operation_information -> serialize_function = serialize_get_pokemon;
    operation_information -> deserialize_function = deserialize_get_pokemon;

    return operation_information;
}

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

void initialize_processes_information(){

    processes_information = list_create();
    list_add(processes_information, (void*) team_process_information());
    list_add(processes_information, (void*) broker_process_information());
    list_add(processes_information, (void*) gamecard_process_information());
}

t_process_information* process_information_named(char* process_name){

    bool _is_valid_process(void* process_information){
        return string_equals_ignore_case(((t_process_information*) process_information) -> name, process_name);
    }
    return list_find(processes_information, _is_valid_process);
}

t_list* all_operations(){
    t_list* operations = list_create();

    void _add_operation(void* process_information){
        list_add_all(operations, ((t_process_information*) process_information) -> operations);
    }
    list_iterate(processes_information, _add_operation);
    return operations;
}

t_operation_information* operation_information_with_code(uint32_t operation_code){
    t_list* operations = all_operations();

    bool _is_for(void* operation_information){
        return ((t_operation_information*) operation_information) -> code == operation_code;
    }
    t_operation_information* operation_information_found = list_find(operations, _is_for);

    list_destroy(operations);
    return operation_information_found;
}

void free_processes_information(){
    //TODO
}