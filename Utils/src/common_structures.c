#include "../include/common_structures.h"
#include <stdlib.h>
#include <commons/string.h>
#include <operations_information.h>

void free_request(t_request* request){
    free(request->structure);
    free(request);
}

void free_process_information(t_process_information* process_information){
    list_destroy_and_destroy_elements(process_information -> operations, (void (*)(void *)) free_operation_information);
    free(process_information);
}

void free_operation_information(t_operation_information* operation_information){
    free(operation_information);
}

void free_char_array(char** char_array){
    //itero hasta que lo que apunta char array sea null, en cuyo caso null evalua como false
    for (int i = 0; char_array[i]; i++ ){
        free( char_array[i] );
    }
    free( char_array );
}

void free_serialization_information(t_serialization_information* serialization_information){
    free(serialization_information -> serialized_request);
    free(serialization_information);
}

char* appeared_pokemon_as_string(t_appeared_pokemon* appeared_pokemon){

    return string_from_format("operacion: APPEARED_POKEMON\n Pokemon: %s, pos_x: %d, pos_y: %d, message_id: %d",appeared_pokemon->pokemon_name, appeared_pokemon->pos_x, appeared_pokemon->pos_y, appeared_pokemon->message_id);
}

char* new_pokemon_as_string(t_new_pokemon* new_pokemon){

    return string_from_format("operacion: NEW_POKEMON\n Pokemon: %s, pos_x: %d, pos_y: %d, quantity: %d, message_id: %d",new_pokemon->pokemon_name, new_pokemon->pos_x, new_pokemon->pos_y, new_pokemon->quantity, new_pokemon->message_id);
}

char* catch_pokemon_as_string(t_catch_pokemon* catch_pokemon){

    return string_from_format("operacion: CATCH_POKEMON\n Pokemon: %s, pos_x: %d, pos_y: %d, message_id: %d", catch_pokemon->pokemon_name, catch_pokemon->pos_x, catch_pokemon->pos_y, catch_pokemon->message_id);
}

char* caught_pokemon_as_string(t_caught_pokemon* caught_pokemon){

    return string_from_format("operacion: CAUGHT_POKEMON\n message_id: %d, caught_status: %d", caught_pokemon->message_id, caught_pokemon->caught_status);
}

char* get_pokemon_as_string(t_get_pokemon* get_pokemon){

    return string_from_format("operacion: GET_POKEMON\n Pokemon: %s", get_pokemon -> pokemon_name);
}

char* subscribe_me_as_string(t_subscribe_me* subscribe_me){
    t_operation_information * operation_information = operation_information_with_code(subscribe_me -> operation_queue);
    return string_from_format("operacion: SUBSCRIBE_ME\n Queue: %s", operation_information->name);
}

void free_printable_object(t_printable_object* printable_object){
    free(printable_object -> object);
    free(printable_object);
}