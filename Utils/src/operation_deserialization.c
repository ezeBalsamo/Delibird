#include "../include/operation_deserialization.h"
#include <stdlib.h>
#include <string.h>
#include <serialization_interface.h>
#include <serializable_objects.h>

t_request* deserialize(void* serialized_request){
    uint32_t operation;
    void* serialized_structure;
    uint32_t serialized_structure_size;

    int offset = 0;

    memcpy(&operation, serialized_request + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&serialized_structure_size, serialized_request + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    serialized_structure = malloc(serialized_structure_size);
    memcpy(serialized_structure, serialized_request + offset, serialized_structure_size);

    t_serializable_object* serializable_object = serializable_object_with_code(operation);
    return (*(serializable_object -> deserialize_function)) (serialized_structure);
}

t_request* deserialize_new_pokemon(void* serialized_structure) {

    uint32_t pokemon_name_length;
    char* pokemon_name;
    uint32_t pos_x;
    uint32_t pos_y;
    uint32_t quantity;

    uint32_t offset = 0;

    memcpy(&pokemon_name_length, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    pokemon_name = malloc(pokemon_name_length);
    memcpy(pokemon_name, serialized_structure + offset, pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(&pos_x, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&pos_y, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&quantity, serialized_structure + offset, sizeof(uint32_t));

    t_new_pokemon* new_pokemon = malloc(sizeof(t_new_pokemon));
    new_pokemon -> pokemon_name = pokemon_name;
    new_pokemon -> pos_x = pos_x;
    new_pokemon -> pos_y = pos_y;
    new_pokemon -> quantity = quantity;

    t_request* request = malloc(sizeof(t_request));
    request -> operation = NEW_POKEMON;
    request -> structure = (void*) new_pokemon;
    request -> sanitizer_function = free;
    return request;
}

t_request* deserialize_appeared_pokemon(void* serialized_structure) {

    uint32_t pokemon_name_length;
    char* pokemon_name;
    uint32_t pos_x;
    uint32_t pos_y;

    uint32_t offset = 0;

    memcpy(&pokemon_name_length, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    pokemon_name = malloc(pokemon_name_length);
    memcpy(pokemon_name, serialized_structure + offset, pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(&pos_x, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&pos_y, serialized_structure + offset, sizeof(uint32_t));

    t_appeared_pokemon* appeared_pokemon = malloc(sizeof(t_appeared_pokemon));
    appeared_pokemon -> pokemon_name = pokemon_name;
    appeared_pokemon -> pos_x = pos_x;
    appeared_pokemon -> pos_y = pos_y;

    t_request* request = malloc(sizeof(t_request));
    request -> operation = APPEARED_POKEMON;
    request -> structure = (void*) appeared_pokemon;
    request -> sanitizer_function = free;
    return request;
}

t_request* deserialize_get_pokemon(void* serialized_structure){
    //Mensaje GET envia unicamente el pokemon
    uint32_t pokemon_name_length;
    char* pokemon_name;
    uint32_t offset = 0;

    memcpy(&pokemon_name_length, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    pokemon_name = malloc(pokemon_name_length);
    memcpy(pokemon_name, serialized_structure + offset, pokemon_name_length);

    t_get_pokemon* get_pokemon = malloc(sizeof(t_get_pokemon));
    get_pokemon -> pokemon_name = pokemon_name;

    t_request* request = malloc(sizeof(t_request));
    request -> operation = GET_POKEMON;
    request -> structure = (void*) get_pokemon;
    request -> sanitizer_function = free;

    return request;
}

t_request* deserialize_localized_pokemon(void* serialized_structure){
    uint32_t pokemon_name_length;
    char* pokemon_name;
    uint32_t quantity;
    t_list* positions = list_create();
    uint32_t offset = 0;

    memcpy(&pokemon_name_length, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    pokemon_name = malloc(pokemon_name_length);
    memcpy(pokemon_name, serialized_structure + offset, pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(&quantity, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    for(int i = 0; i < quantity; i++){
        uint32_t* position = malloc(sizeof(uint32_t));
        memcpy(position, serialized_structure + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        list_add(positions, (void *) position);
    }

    t_localized_pokemon* localized_pokemon = malloc(sizeof(t_localized_pokemon));
    localized_pokemon -> pokemon_name = pokemon_name;
    localized_pokemon -> quantity = quantity;
    localized_pokemon -> positions = positions;

    t_request* request = malloc(sizeof(t_request));
    request -> operation = LOCALIZED_POKEMON;
    request -> structure = (void*) localized_pokemon;
    request -> sanitizer_function = (void (*)(void *)) free_localized_pokemon;
    return request;
}

t_request* deserialize_catch_pokemon(void* serialized_structure) {

    uint32_t pokemon_name_length;
    char* pokemon_name;
    uint32_t pos_x;
    uint32_t pos_y;

    uint32_t offset = 0;

    memcpy(&pokemon_name_length, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    pokemon_name = malloc(pokemon_name_length);
    memcpy(pokemon_name, serialized_structure + offset, pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(&pos_x, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&pos_y, serialized_structure + offset, sizeof(uint32_t));

    t_catch_pokemon* catch_pokemon = malloc(sizeof(t_catch_pokemon));
    catch_pokemon -> pokemon_name = pokemon_name;
    catch_pokemon -> pos_x = pos_x;
    catch_pokemon -> pos_y = pos_y;

    t_request* request = malloc(sizeof(t_request));
    request -> operation = CATCH_POKEMON;
    request -> structure = (void*) catch_pokemon;
    request -> sanitizer_function = free;
    return request;
}

t_request* deserialize_caught_pokemon(void* serialized_structure){

    uint32_t caught_status;    // 0/1 - FAIL/OK
    uint32_t offset = 0;

    memcpy(&caught_status, serialized_structure + offset, sizeof(uint32_t));

    t_caught_pokemon* caught_pokemon = malloc(sizeof(t_catch_pokemon));
    caught_pokemon->caught_status = caught_status;

    t_request* request = malloc(sizeof(t_request));
    request -> operation = CAUGHT_POKEMON;
    request -> structure = (void*) caught_pokemon;
    request -> sanitizer_function = free;
    return request;
}

t_request* deserialize_subscribe_me(void* serialized_structure){

    uint32_t* operation_queue = malloc(sizeof(uint32_t));

    memcpy(operation_queue, serialized_structure, sizeof(uint32_t));

    t_subscribe_me* subscribe_me = malloc(sizeof(t_subscribe_me));
    subscribe_me -> operation_queue = *operation_queue;


    t_request* request = malloc(sizeof(t_request));
    request -> operation = SUBSCRIBE_ME;
    request -> structure = subscribe_me;
    request -> sanitizer_function = free;
    return request;
}

t_request* deserialize_identified_message(void* serialized_structure){

    uint32_t message_id;
    uint32_t request_serialization_information_amount_of_bytes;
    void *serialized_request;

    uint32_t offset = 0;

    memcpy(&message_id, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&request_serialization_information_amount_of_bytes, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    serialized_request = malloc(request_serialization_information_amount_of_bytes);

    memcpy(serialized_request, serialized_structure + offset, request_serialization_information_amount_of_bytes);

    t_request* internal_request = deserialize(serialized_request);

    t_identified_message* identified_message = malloc(sizeof(t_identified_message));
    identified_message -> message_id = message_id;
    identified_message -> request = internal_request;

    t_request* request = malloc(sizeof(t_request));
    request -> operation = IDENTIFIED_MESSAGE;
    request -> structure = (void*) identified_message;
    request -> sanitizer_function = (void (*)(void *)) free_identified_message;

    return request;
}