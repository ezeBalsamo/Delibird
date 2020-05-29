#include <commons/collections/list.h>
#include <commons/string.h>
#include "../include/operations_information.h"
#include "../include/common_structures.h"
#include "../include/operation_serialization.h"
#include "../include/operation_deserialization.h"
#include <stdlib.h>
#include <t_list_extension.h>

t_list* all_operations;

void initialize_and_load_new_pokemon_information(){
    t_operation_information* operation_information = malloc(sizeof(t_operation_information));
    operation_information -> code = NEW_POKEMON;
    operation_information -> name = "NEW_POKEMON";
    operation_information -> max_arguments_amount = 8;
    operation_information -> serialize_function = serialize_new_pokemon;
    operation_information -> deserialize_function = deserialize_new_pokemon;

    list_add(all_operations, (void*) operation_information);
}

void initialize_and_load_appeared_pokemon_information(){
    t_operation_information* operation_information = malloc(sizeof(t_operation_information));
    operation_information -> code = APPEARED_POKEMON;
    operation_information -> name = "APPEARED_POKEMON";
    operation_information -> max_arguments_amount = 7;
    operation_information -> serialize_function = serialize_appeared_pokemon;
    operation_information -> deserialize_function = deserialize_appeared_pokemon;

    list_add(all_operations, (void*) operation_information);
}

void initialize_and_load_catch_pokemon_information(){
    t_operation_information* operation_information = malloc(sizeof(t_operation_information));
    operation_information -> code = CATCH_POKEMON;
    operation_information -> name = "CATCH_POKEMON";
    operation_information -> max_arguments_amount = 7;
    operation_information -> serialize_function = serialize_catch_pokemon;
    operation_information -> deserialize_function = deserialize_catch_pokemon;

    list_add(all_operations, (void*) operation_information);
}

void initialize_and_load_caught_pokemon_information(){
    t_operation_information* operation_information = malloc(sizeof(t_operation_information));
    operation_information -> code = CAUGHT_POKEMON;
    operation_information -> name = "CAUGHT_POKEMON";
    operation_information -> max_arguments_amount = 5;
    operation_information -> serialize_function = serialize_caught_pokemon;
    operation_information -> deserialize_function = deserialize_caught_pokemon;

    list_add(all_operations, (void*) operation_information);
}

void initialize_and_load_get_pokemon_information(){
    t_operation_information* operation_information = malloc(sizeof(t_operation_information));
    operation_information -> code = GET_POKEMON;
    operation_information -> name = "GET_POKEMON";
    operation_information -> max_arguments_amount = 5;
    operation_information -> serialize_function = serialize_get_pokemon;
    operation_information -> deserialize_function = deserialize_get_pokemon;

    list_add(all_operations, (void*) operation_information);
}

void initialize_and_load_localized_pokemon_information(){
    t_operation_information* operation_information = malloc(sizeof(t_operation_information));
    operation_information -> code = LOCALIZED_POKEMON;
    operation_information -> name = "LOCALIZED_POKEMON";
    operation_information -> max_arguments_amount = 4;
    operation_information -> serialize_function = serialize_localized_pokemon;
    operation_information -> deserialize_function = deserialize_localized_pokemon;

    list_add(all_operations, (void*) operation_information);
}

void initialize_and_load_subscribe_me_operation_information(){
    t_operation_information* operation_information = malloc(sizeof(t_operation_information));
    operation_information -> code = SUBSCRIBE_ME;
    operation_information -> name = "SUBSCRIBE_ME";
    operation_information -> max_arguments_amount = 4;
    operation_information -> serialize_function = serialize_subscribe_me;
    operation_information -> deserialize_function = deserialize_subscribe_me;

    list_add(all_operations, (void*) operation_information);

}

void initialize_and_load_identified_message_operation_information(){
    t_operation_information* operation_information = malloc(sizeof(t_operation_information));
    operation_information -> code = IDENTIFIED_MESSAGE;
    operation_information -> name = "IDENTIFIED_MESSAGE";
    operation_information -> max_arguments_amount = 0;
    operation_information -> serialize_function = serialize_identified_message;
    operation_information -> deserialize_function = deserialize_identified_message;

    list_add(all_operations, (void*) operation_information);
}

void initialize_operations_information(){

    all_operations = list_create();
    initialize_and_load_new_pokemon_information();
    initialize_and_load_appeared_pokemon_information();
    initialize_and_load_get_pokemon_information();
    initialize_and_load_localized_pokemon_information();
    initialize_and_load_catch_pokemon_information();
    initialize_and_load_caught_pokemon_information();
    initialize_and_load_subscribe_me_operation_information();
    initialize_and_load_identified_message_operation_information();

}

t_operation_information* new_pokemon_operation_information(){
    return operation_information_with_code(NEW_POKEMON);
}

t_operation_information* appeared_pokemon_operation_information(){
    return operation_information_with_code(APPEARED_POKEMON);
}

t_operation_information* catch_pokemon_operation_information(){
    return operation_information_with_code(CATCH_POKEMON);
}

t_operation_information* caught_pokemon_operation_information(){
    return operation_information_with_code(CAUGHT_POKEMON);
}

t_operation_information* get_pokemon_operation_information(){
    return operation_information_with_code(GET_POKEMON);
}

t_operation_information* localized_pokemon_operation_information(){
    return operation_information_with_code(LOCALIZED_POKEMON);
}

t_operation_information* subscribe_me_operation_information(){
    return operation_information_with_code(SUBSCRIBE_ME);
}


t_list* all_queue_operations() {

    t_list *queue_operations = list_create();
    list_add(queue_operations, new_pokemon_operation_information());
    list_add(queue_operations, appeared_pokemon_operation_information());
    list_add(queue_operations, get_pokemon_operation_information());
    list_add(queue_operations, localized_pokemon_operation_information());
    list_add(queue_operations, catch_pokemon_operation_information());
    list_add(queue_operations, caught_pokemon_operation_information());

    return queue_operations;
}

uint32_t queue_code_of(char* queue_name){
    t_list* queue_operations = all_queue_operations();

    bool _is_for(void* operation_information){
        t_operation_information* cast_operation_information = (t_operation_information*) operation_information;
        return string_equals_ignore_case(cast_operation_information -> name, queue_name);
    }

    t_operation_information* queue_operation_information = list_find(queue_operations, _is_for);

    uint32_t queue_code = queue_operation_information -> code;

    list_destroy(queue_operations);

    return queue_code;
}

bool exists_queue_named(char* queue_name){
    t_list* queue_operations = all_queue_operations();

    bool _is_for(void* operation_information){
        t_operation_information* cast_operation_information = (t_operation_information*) operation_information;
        return string_equals_ignore_case(cast_operation_information -> name, queue_name);
    }

    bool queue_exists = list_any_satisfy(queue_operations, _is_for);

    list_destroy(queue_operations);

    return queue_exists;
}

char* queue_name_of(uint32_t queue_code){
    return operation_information_with_code(queue_code) -> name;
}

t_operation_information* operation_information_with_code(uint32_t operation_code){

    bool _is_for(void* operation_information){
        return ((t_operation_information*) operation_information) -> code == operation_code;
    }
    t_operation_information* operation_information_found = list_find(all_operations, _is_for);

    return operation_information_found;
}

void free_operations_information(){
    list_destroy_and_destroy_elements(all_operations, free);
}