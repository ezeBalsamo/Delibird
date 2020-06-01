#include <pretty_printer.h>
#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <serializable_objects.h>
#include <queue_code_name_associations.h>
#include <general_logs.h>

t_list* printable_objects;
static char *reset_color = "\x1b[0m";

char* new_pokemon_as_string(t_new_pokemon* new_pokemon){

    return string_from_format("Operación: NEW_POKEMON\nArgumentos: Pokemon: %s, pos_x: %d, pos_y: %d, quantity: %d",
            new_pokemon -> pokemon_name, new_pokemon -> pos_x, new_pokemon -> pos_y, new_pokemon -> quantity);
}

char* appeared_pokemon_as_string(t_appeared_pokemon* appeared_pokemon){

    return string_from_format("Operación: APPEARED_POKEMON\nArgumentos: Pokemon: %s, pos_x: %d, pos_y: %d",
            appeared_pokemon -> pokemon_name, appeared_pokemon -> pos_x, appeared_pokemon -> pos_y);
}

char* get_pokemon_as_string(t_get_pokemon* get_pokemon){

    return string_from_format("Operación: GET_POKEMON\nArgumentos: Pokemon: %s",
            get_pokemon -> pokemon_name);
}

char* localized_pokemon_as_string(void* to_be_defined){
    //TODO to be defined
}

char* catch_pokemon_as_string(t_catch_pokemon* catch_pokemon){

    return string_from_format("Operación: CATCH_POKEMON\nArgumentos: Pokemon: %s, pos_x: %d, pos_y: %d",
            catch_pokemon -> pokemon_name, catch_pokemon -> pos_x, catch_pokemon -> pos_y);
}

char* caught_pokemon_as_string(t_caught_pokemon* caught_pokemon){

    return string_from_format("Operación: CAUGHT_POKEMON\nArgumentos: caught_status: %d",
            caught_pokemon -> caught_status);
}

char* subscribe_me_as_string(t_subscribe_me* subscribe_me){

    return string_from_format("Operación: SUBSCRIBE_ME\nArgumentos: %s",
            queue_name_of(subscribe_me -> operation_queue));
}

char* identified_message_as_string(t_identified_message* identified_message){

    char* request_as_string = request_pretty_print(identified_message -> request);
    char* message = string_from_format("Operación: IDENTIFIED_MESSAGE\nArgumentos: message_id: %d\n%s",
            identified_message -> message_id, request_as_string);

    free(request_as_string);
    return message;
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
    printable_object -> code = CAUGHT_POKEMON;
    printable_object -> print_function = (char *(*)(void *)) caught_pokemon_as_string;

    list_add(printable_objects, (void*) printable_object);
}

void initialize_and_load_subscribe_me_pretty_print(){
    t_printable_object* printable_object = malloc(sizeof(t_printable_object));
    printable_object -> code = SUBSCRIBE_ME;
    printable_object -> print_function = (char *(*)(void *)) subscribe_me_as_string;

    list_add(printable_objects, (void*) printable_object);
}

void initialize_and_load_identified_message(){
    t_printable_object* printable_object = malloc(sizeof(t_printable_object));
    printable_object -> code = IDENTIFIED_MESSAGE;
    printable_object -> print_function = (char *(*)(void *)) identified_message_as_string;

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
    initialize_and_load_identified_message();

    log_succesful_initialize_pretty_printer();
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

char* change_message_colour(char* message, char* colour){
    message = string_from_format("%s%s%s", colour, message, reset_color);
    return message;
}

void free_pretty_printer(){
    list_destroy_and_destroy_elements(printable_objects, free);
}