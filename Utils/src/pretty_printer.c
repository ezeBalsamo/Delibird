#include <pretty_printer.h>
#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <serializable_objects.h>
#include <queue_code_name_associations.h>
#include <general_logs.h>
#include <garbage_collector.h>

t_list* printable_objects;

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

char* localized_pokemon_as_string(t_localized_pokemon* localized_pokemon){
    char* message_without_positions =
            string_from_format("Operación: %s\nArgumentos: Pokemon: %s, cantidad: %d, ",
                    queue_name_of(LOCALIZED_POKEMON),
                    localized_pokemon -> pokemon_name,
                    localized_pokemon -> quantity);

    char* message_with_position_quantity;

    if(localized_pokemon -> quantity == 1){
        message_with_position_quantity = string_from_format("%sPosición: ", message_without_positions);
    }else{
        message_with_position_quantity = string_from_format("%sPosiciones: ", message_without_positions);
    }

    char* message_with_positions = string_new();
    string_append(&message_with_positions, message_with_position_quantity);

    for(int i = 0; i < (localized_pokemon -> quantity) * 2; i+=2){
        void* x_position = list_get(localized_pokemon -> positions, i);
        uint32_t cast_x_position = *((uint32_t*) x_position);
        char* printable_x_position = string_itoa(cast_x_position);

        void* y_position = list_get(localized_pokemon -> positions, i + 1);
        uint32_t cast_y_position = *((uint32_t*) y_position);
        char* printable_y_position = string_itoa(cast_y_position);

        string_append(&message_with_positions, "(");
        string_append(&message_with_positions, printable_x_position);
        string_append(&message_with_positions, ", ");
        string_append(&message_with_positions, printable_y_position);
        string_append(&message_with_positions, ")");

        int index_to_know_remaining_positions = i + 2;
        if(index_to_know_remaining_positions < ((localized_pokemon -> quantity) * 2)){
            string_append(&message_with_positions, ", ");
        }

        free(printable_x_position);
        free(printable_y_position);
    }

    free(message_without_positions);
    free(message_with_position_quantity);

    return message_with_positions;
}

char* catch_pokemon_as_string(t_catch_pokemon* catch_pokemon){

    return string_from_format("Operación: CATCH_POKEMON\nArgumentos: Pokemon: %s, pos_x: %d, pos_y: %d",
            catch_pokemon -> pokemon_name, catch_pokemon -> pos_x, catch_pokemon -> pos_y);
}

char* caught_pokemon_as_string(t_caught_pokemon* caught_pokemon){
    char* caught_status_to_print;

    if(caught_pokemon -> caught_status == 1){
       caught_status_to_print = "OK";
    } else {
        caught_status_to_print = "FAIL";
    }
    return string_from_format("Operación: CAUGHT_POKEMON\nArgumentos: caught_status: %s",
            caught_status_to_print);
}

char* subscribe_me_as_string(t_subscribe_me* subscribe_me){

    return string_from_format("Operación: SUBSCRIBE_ME\nArgumentos: %s",
            queue_name_of(subscribe_me -> operation_queue));
}

bool is_correlative_identified_message(t_identified_message* identified_message){
    return identified_message -> request -> operation == IDENTIFIED_MESSAGE;
}

char* correlative_identified_message_as_string(t_identified_message* correlative_identified_message){
    t_identified_message* original_identified_message = (t_identified_message*) correlative_identified_message -> request -> structure;

    char * request_as_string = request_pretty_print(original_identified_message -> request);
    char* message =
            string_from_format("Operación: CORRELATIVE_IDENTIFIED_MESSAGE\nArgumentos:\n"
                               "\t-> message_id: %d\n\t-> Operación: IDENTIFIED_MESSAGE\n"
                               "\t   Argumentos:\n\t\t   -> message_id: %d\n\t\t   -> %s",
                               correlative_identified_message -> message_id,
                               original_identified_message -> message_id,
                               request_as_string);

    free(request_as_string);
    return message;
}

char* original_identified_message_as_string(t_identified_message* identified_message){
    char* request_as_string = request_pretty_print(identified_message -> request);
    char* message =
            string_from_format("Operación: IDENTIFIED_MESSAGE\nArgumentos:\n\t-> message_id: %d\n\t-> %s",
                               identified_message -> message_id, request_as_string);

    free(request_as_string);

    return message;

}

char* identified_message_as_string(t_identified_message* identified_message){

    if(is_correlative_identified_message(identified_message)){
        return correlative_identified_message_as_string(identified_message);
    }else{
        return original_identified_message_as_string(identified_message);
    }
}

void initialize_and_load_new_pokemon_pretty_print(){
    t_printable_object* printable_object = safe_malloc(sizeof(t_printable_object));
    printable_object -> code = NEW_POKEMON;
    printable_object -> print_function = (char *(*)(void *)) new_pokemon_as_string;

    list_add(printable_objects, (void*) printable_object);
}

void initialize_and_load_appeared_pokemon_pretty_print(){
    t_printable_object* printable_object = safe_malloc(sizeof(t_printable_object));
    printable_object -> code = APPEARED_POKEMON;
    printable_object -> print_function = (char *(*)(void *)) appeared_pokemon_as_string;

    list_add(printable_objects, (void*) printable_object);
}

void initialize_and_load_get_pokemon_pretty_print(){
    t_printable_object* printable_object = safe_malloc(sizeof(t_printable_object));
    printable_object -> code = GET_POKEMON;
    printable_object -> print_function = (char *(*)(void *)) get_pokemon_as_string;

    list_add(printable_objects, (void*) printable_object);
}

void initialize_and_load_localized_pokemon_pretty_print(){
    t_printable_object* printable_object = safe_malloc(sizeof(t_printable_object));
    printable_object -> code = LOCALIZED_POKEMON;
    printable_object -> print_function = (char *(*)(void *)) localized_pokemon_as_string;

    list_add(printable_objects, (void*) printable_object);
}

void initialize_and_load_catch_pokemon_pretty_print(){
    t_printable_object* printable_object = safe_malloc(sizeof(t_printable_object));
    printable_object -> code = CATCH_POKEMON;
    printable_object -> print_function = (char *(*)(void *)) catch_pokemon_as_string;

    list_add(printable_objects, (void*) printable_object);
}

void initialize_and_load_caught_pokemon_pretty_print(){
    t_printable_object* printable_object = safe_malloc(sizeof(t_printable_object));
    printable_object -> code = CAUGHT_POKEMON;
    printable_object -> print_function = (char *(*)(void *)) caught_pokemon_as_string;

    list_add(printable_objects, (void*) printable_object);
}

void initialize_and_load_subscribe_me_pretty_print(){
    t_printable_object* printable_object = safe_malloc(sizeof(t_printable_object));
    printable_object -> code = SUBSCRIBE_ME;
    printable_object -> print_function = (char *(*)(void *)) subscribe_me_as_string;

    list_add(printable_objects, (void*) printable_object);
}

void initialize_and_load_identified_message(){
    t_printable_object* printable_object = safe_malloc(sizeof(t_printable_object));
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
        log_printable_object_not_found_error();
        free_system();
    }

    return (*(printable_object_found -> print_function)) (structure);
}

char* request_pretty_print(t_request* request){
    return pretty_print_of(request -> operation, request -> structure);
}

char* reset_colour(){
    return "\x1b[0m";
}

char* blue_colour(){
    return "\x1b[36m";
}

char* change_message_colour(char* message, char* colour){
    return string_from_format("%s%s%s", colour, message, reset_colour());
}

void free_pretty_printer(){
    list_destroy_and_destroy_elements(printable_objects, free);
}