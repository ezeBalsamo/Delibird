#include "../include/operation_deserialization.h"
#include <stdlib.h>
#include <string.h>

t_request* deserialize_new_pokemon(void* serialized_structure) {

    uint32_t pokemon_name_length;
    char* pokemon_name;
    uint32_t pos_x;
    uint32_t pos_y;
    uint32_t quantity;
    uint32_t message_id;

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
    offset += sizeof(uint32_t);
    memcpy(&message_id, serialized_structure + offset, sizeof(uint32_t));


    t_new_pokemon* new_pokemon = malloc(sizeof(t_new_pokemon));
    new_pokemon -> pokemon_name = pokemon_name;
    new_pokemon -> pos_x = pos_x;
    new_pokemon -> pos_y = pos_y;
    new_pokemon -> quantity = quantity;
    new_pokemon -> message_id = message_id;

    t_request* request = malloc(sizeof(t_request));
    request -> operation = NEW_POKEMON;
    request -> structure = new_pokemon;
    return request;
}

t_request* deserialize_appeared_pokemon(void* serialized_structure) {

    uint32_t pokemon_name_length;
    char* pokemon_name;
    uint32_t pos_x;
    uint32_t pos_y;
    uint32_t message_id;

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
    memcpy(&message_id, serialized_structure + offset, sizeof(uint32_t));

    t_appeared_pokemon* appeared_pokemon = malloc(sizeof(t_appeared_pokemon));
    appeared_pokemon -> pokemon_name = pokemon_name;
    appeared_pokemon -> pos_x = pos_x;
    appeared_pokemon -> pos_y = pos_y;
    appeared_pokemon -> message_id = message_id;

    t_request* request = malloc(sizeof(t_request));
    request -> operation = APPEARED_POKEMON;
    request -> structure = appeared_pokemon;
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
    request -> structure = get_pokemon;

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
    memcpy(positions,serialized_structure + offset, sizeof(t_list));

    t_localized_pokemon* localized_pokemon = malloc(sizeof(t_localized_pokemon));
    localized_pokemon -> pokemon_name = pokemon_name;
    localized_pokemon -> quantity = quantity;
    localized_pokemon -> positions = positions;

    t_request* request = malloc(sizeof(t_request));
    request -> operation = LOCALIZED_POKEMON;
    request -> structure = localized_pokemon;
    return NULL;
}

t_request* deserialize_catch_pokemon(void* serialized_structure) {

    uint32_t pokemon_name_length;
    char* pokemon_name;
    uint32_t pos_x;
    uint32_t pos_y;
    uint32_t message_id;

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
    memcpy(&message_id, serialized_structure + offset, sizeof(uint32_t));

    t_catch_pokemon* catch_pokemon = malloc(sizeof(t_catch_pokemon));
    catch_pokemon -> pokemon_name = pokemon_name;
    catch_pokemon -> pos_x = pos_x;
    catch_pokemon -> pos_y = pos_y;
    catch_pokemon -> message_id = message_id;

    t_request* request = malloc(sizeof(t_request));
    request -> operation = CATCH_POKEMON;
    request -> structure = catch_pokemon;
    return request;
}

t_request* deserialize_caught_pokemon(void* serialized_structure){

    uint32_t message_id;
    uint32_t caught_status;    // 0/1 - FAIL/OK
    uint32_t offset = 0;

    memcpy(&message_id, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&caught_status, serialized_structure + offset, sizeof(uint32_t));

    t_caught_pokemon* caught_pokemon = malloc(sizeof(t_catch_pokemon));
    caught_pokemon -> message_id = message_id;
    caught_pokemon->caught_status = caught_status;

    t_request* request = malloc(sizeof(t_request));
    request -> operation = CAUGHT_POKEMON;
    request -> structure = caught_pokemon;
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
    return request;
}