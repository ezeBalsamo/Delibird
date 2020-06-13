#include "../include/entry_point_processes_information.h"
#include "../include/entry_point_logs_manager.h"
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include "../../Utils/include/garbage_collector.h"
#include <commons/string.h>
#include <stdlib.h>


t_list* processes_information;

t_operation_information* new_pokemon_operation_information(uint32_t arguments_amount, bool has_identified_message){
    t_operation_information* operation_information = safe_malloc(sizeof(t_operation_information));
    operation_information -> serializable_object = serializable_new_pokemon();
    operation_information -> name = queue_name_of(NEW_POKEMON);
    operation_information -> arguments_amount = arguments_amount;
    operation_information -> has_identified_message = has_identified_message;

    return operation_information;
}

t_operation_information* appeared_pokemon_operation_information(uint32_t arguments_amount, bool has_identified_message){
    t_operation_information* operation_information = safe_malloc(sizeof(t_operation_information));
    operation_information -> serializable_object = serializable_appeared_pokemon();
    operation_information -> name = queue_name_of(APPEARED_POKEMON);
    operation_information -> arguments_amount = arguments_amount;
    operation_information -> has_identified_message = has_identified_message;

    return operation_information;
}

t_operation_information* get_pokemon_operation_information(uint32_t arguments_amount, bool has_identified_message){
    t_operation_information* operation_information = safe_malloc(sizeof(t_operation_information));
    operation_information -> serializable_object = serializable_get_pokemon();
    operation_information -> name = queue_name_of(GET_POKEMON);
    operation_information -> arguments_amount = arguments_amount;
    operation_information -> has_identified_message = has_identified_message;

    return operation_information;
}

t_operation_information* catch_pokemon_operation_information(uint32_t arguments_amount, bool has_identified_message){
    t_operation_information* operation_information = safe_malloc(sizeof(t_operation_information));
    operation_information -> serializable_object = serializable_catch_pokemon();
    operation_information -> name = queue_name_of(CATCH_POKEMON);
    operation_information -> arguments_amount = arguments_amount;
    operation_information -> has_identified_message = has_identified_message;

    return operation_information;
}

t_operation_information* caught_pokemon_operation_information(){
    t_operation_information* operation_information = safe_malloc(sizeof(t_operation_information));
    operation_information -> serializable_object = serializable_caught_pokemon();
    operation_information -> name = queue_name_of(CAUGHT_POKEMON);
    operation_information -> arguments_amount = 5;
    operation_information -> has_identified_message = true;

    return operation_information;
}

t_operation_information* subscribe_operation_information(){
    t_operation_information* operation_information = safe_malloc(sizeof(t_operation_information));
    operation_information -> serializable_object = serializable_subscribe_me();
    operation_information -> name = "SUSCRIBE_ME";
    operation_information -> arguments_amount = 4;
    operation_information -> has_identified_message = false;
}

t_list* team_operations_information(){
    t_list* operations = list_create();
    list_add(operations, appeared_pokemon_operation_information(8, true));
    return operations;
}

t_list* broker_operations_information(){
    t_list* operations = list_create();
    list_add(operations, new_pokemon_operation_information(7, false));
    list_add(operations, appeared_pokemon_operation_information(7, true));
    list_add(operations, get_pokemon_operation_information(4, false));
    list_add(operations, catch_pokemon_operation_information(6, false));
    list_add(operations, caught_pokemon_operation_information());
    return operations;
}

t_list* gamecard_operations_information(){
    t_list* operations = list_create();
    list_add(operations, new_pokemon_operation_information(8, true));
    list_add(operations, get_pokemon_operation_information(5, true));
    list_add(operations, catch_pokemon_operation_information(7, true));
    return operations;
}

t_list* suscriptor_operations_information(){
    t_list* operations = list_create();
    list_add(operations, subscribe_operation_information());
    return operations;
}

t_process_information* team_process_information(){
    t_process_information* process_information = safe_malloc(sizeof(t_process_information));
    process_information -> code = TEAM;
    process_information -> name = "TEAM";
    process_information -> operations = team_operations_information();

    return process_information;
}

t_process_information* broker_process_information(){
    t_process_information* process_information = safe_malloc(sizeof(t_process_information));
    process_information -> code = BROKER;
    process_information -> name = "BROKER";
    process_information -> operations = broker_operations_information();

    return process_information;
}

t_process_information* gamecard_process_information(){
    t_process_information* process_information = safe_malloc(sizeof(t_process_information));
    process_information -> code = GAMECARD;
    process_information -> name = "GAMECARD";
    process_information -> operations = gamecard_operations_information();

    return process_information;
}

t_process_information* suscriptor_process_information(){
    t_process_information* process_information = safe_malloc(sizeof(t_process_information));
    process_information -> code = SUSCRIPTOR;
    process_information -> name = "SUSCRIPTOR";
    process_information -> operations = suscriptor_operations_information();

    return process_information;
}

void initialize_processes_information(){

    initialize_serializable_objects();

    processes_information = list_create();
    list_add(processes_information, (void*) team_process_information());
    list_add(processes_information, (void*) broker_process_information());
    list_add(processes_information, (void*) gamecard_process_information());
    list_add(processes_information, (void*) suscriptor_process_information());
}

t_process_information* process_information_with_code(uint32_t code){

    bool _is_for(void* process_information){
        return ((t_process_information*) process_information) -> code == code;
    }
    return list_find(processes_information, _is_for);
}

char* broker_process_name(){
    t_process_information* broker_process_information = process_information_with_code(BROKER);
    return broker_process_information -> name;
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
        free_system();
    }

    return process_information -> code;
}

void free_process_information(t_process_information* self){
    list_destroy_and_destroy_elements(self -> operations, (void (*)(void *)) free);
    free(self);
}

void free_processes_information(){
    list_destroy_and_destroy_elements(processes_information, (void (*)(void *)) free_process_information);
}