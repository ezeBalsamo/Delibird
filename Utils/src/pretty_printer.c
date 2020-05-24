#include <pretty_printer.h>
#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <operations_information.h>

t_list* printable_objects;

char* new_pokemon_as_string(t_new_pokemon* new_pokemon){

    return string_from_format("Operación: NEW_POKEMON\nArgumentos: Pokemon: %s, pos_x: %d, pos_y: %d, quantity: %d, message_id: %d",
                              new_pokemon->pokemon_name, new_pokemon->pos_x, new_pokemon->pos_y, new_pokemon->quantity, new_pokemon->message_id);
}

char* appeared_pokemon_as_string(t_appeared_pokemon* appeared_pokemon){

    return string_from_format("Operación: APPEARED_POKEMON\nArgumentos: Pokemon: %s, pos_x: %d, pos_y: %d, message_id: %d",
                              appeared_pokemon->pokemon_name, appeared_pokemon->pos_x, appeared_pokemon->pos_y, appeared_pokemon->message_id);
}

char* get_pokemon_as_string(t_get_pokemon* get_pokemon){

    return string_from_format("Operación: GET_POKEMON\nArgumentos: Pokemon: %s, message_id: %d",
                              get_pokemon -> pokemon_name, get_pokemon -> message_id);
}

char* localized_pokemon_as_string(void* to_be_defined){
    //TODO to be defined
}

char* catch_pokemon_as_string(t_catch_pokemon* catch_pokemon){

    return string_from_format("Operación: CATCH_POKEMON\nArgumentos: Pokemon: %s, pos_x: %d, pos_y: %d, message_id: %d",
                              catch_pokemon->pokemon_name, catch_pokemon->pos_x, catch_pokemon->pos_y, catch_pokemon->message_id);
}

char* caught_pokemon_as_string(t_caught_pokemon* caught_pokemon){

    return string_from_format("Operación: CAUGHT_POKEMON\nArgumentos: message_id: %d, caught_status: %d",
                              caught_pokemon->message_id, caught_pokemon->caught_status);
}

char* subscribe_me_as_string(t_subscribe_me* subscribe_me){
    t_operation_information* operation_information = operation_information_with_code(subscribe_me -> operation_queue);
    return string_from_format("Operación: SUBSCRIBE_ME\nArgumentos: %s", operation_information->name);
}

void initialize_and_load_new_pokemon_pretty_print(){
    t_printable_object* printable_object = malloc(sizeof(t_printable_object));
    printable_object -> code = NEW_POKEMON;
    printable_object -> print_function = (char *(*)(void *)) new_pokemon_as_string;

    list_add(printable_objects, (void*) printable_object);
}

void initialize_and_load_appeared_pokemon_pretty_print(){
    t_printable_object* printable_object = malloc(sizeof(t_printable_object));
    printable_object -> code = APPEARED_POKEMON;
    printable_object -> print_function = (char *(*)(void *)) appeared_pokemon_as_string;

    list_add(printable_objects, (void*) printable_object);
}

void initialize_and_load_get_pokemon_pretty_print(){
    t_printable_object* printable_object = malloc(sizeof(t_printable_object));
    printable_object -> code = GET_POKEMON;
    printable_object -> print_function = (char *(*)(void *)) get_pokemon_as_string;

    list_add(printable_objects, (void*) printable_object);
}

void initialize_and_load_localized_pokemon_pretty_print(){
    t_printable_object* printable_object = malloc(sizeof(t_printable_object));
    printable_object -> code = LOCALIZED_POKEMON;
    printable_object -> print_function = (char *(*)(void *)) localized_pokemon_as_string;

    list_add(printable_objects, (void*) printable_object);
}

void initialize_and_load_catch_pokemon_pretty_print(){
    t_printable_object* printable_object = malloc(sizeof(t_printable_object));
    printable_object -> code = CATCH_POKEMON;
    printable_object -> print_function = (char *(*)(void *)) catch_pokemon_as_string;

    list_add(printable_objects, (void*) printable_object);
}

void initialize_and_load_caught_pokemon_pretty_print(){
    t_printable_object* printable_object = malloc(sizeof(t_printable_object));
    printable_object -> code = NEW_POKEMON;
    printable_object -> print_function = (char *(*)(void *)) caught_pokemon_as_string;

    list_add(printable_objects, (void*) printable_object);
}

void initialize_and_load_subscribe_me_pretty_print(){
    t_printable_object* printable_object = malloc(sizeof(t_printable_object));
    printable_object -> code = SUBSCRIBE_ME;
    printable_object -> print_function = (char *(*)(void *)) subscribe_me_as_string;

    list_add(printable_objects, (void*) printable_object);
}

void initialize_pretty_printer(){
    printable_objects = list_create();
    initialize_and_load_new_pokemon_pretty_print();
    initialize_and_load_appeared_pokemon_pretty_print();
    initialize_and_load_get_pokemon_pretty_print();
    initialize_and_load_localized_pokemon_pretty_print();
    initialize_and_load_catch_pokemon_pretty_print();
    initialize_and_load_caught_pokemon_pretty_print();
    initialize_and_load_subscribe_me_pretty_print();
}

char* pretty_print_of(uint32_t code, void* structure){

    bool _has_code(void* printable_object){
        t_printable_object* cast_printable_object = (t_printable_object*) printable_object;
        return cast_printable_object -> code == code;
    }

    t_printable_object* printable_object_found = list_find(printable_objects, _has_code);

    if(printable_object_found == NULL){
        //todo
        printf("Romper todo a la bosta, tal vez necesite implementar un logger general");
        exit(EXIT_FAILURE);
    }

    return (*(printable_object_found -> print_function)) (structure);
}

char* request_pretty_print(t_request* request){
    return pretty_print_of(request -> operation, request -> structure);
}

void free_pretty_printer(){
    list_destroy_and_destroy_elements(printable_objects, free);
}