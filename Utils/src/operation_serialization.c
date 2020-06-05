#include "../include/operation_serialization.h"
#include "../include/serializable_objects.h"
#include <string.h>

uint32_t amount_of_bytes_of_new(t_new_pokemon* new_pokemon){
    return sizeof(uint32_t)                                     //Pokemon name length
           + strlen(new_pokemon -> pokemon_name) + 1            //Pokemon name
           + sizeof(uint32_t)                                   //Pos x
           + sizeof(uint32_t)                                   //Pos y
           + sizeof(uint32_t);                                  //Cantidad
}

uint32_t amount_of_bytes_of_appeared(t_appeared_pokemon* appeared_pokemon){
    return sizeof(uint32_t)                                     //Pokemon name length
            + strlen(appeared_pokemon -> pokemon_name) + 1      //Pokemon name
            + sizeof(uint32_t)                                  //Pos x
            + sizeof(uint32_t);                                 //Pos y
}

uint32_t amount_of_bytes_of_get(t_get_pokemon* get_pokemon){
    return sizeof(uint32_t)                                     //Pokemon name length
           + strlen(get_pokemon -> pokemon_name) + 1;    	    //Pokemon name
}

uint32_t amount_of_bytes_of_localized(t_localized_pokemon * localized_pokemon){
    return sizeof(uint32_t)                                                 //Pokemon name length
           + strlen(localized_pokemon -> pokemon_name) + 1                  //Pokemon name
           + sizeof(uint32_t)                                               //quantity
           + (list_size(localized_pokemon->positions) *sizeof(uint32_t));   //positions
}

uint32_t amount_of_bytes_of_catch(t_catch_pokemon* catch_pokemon){
    return sizeof(uint32_t)                                     //Pokemon name length
           + strlen(catch_pokemon -> pokemon_name) + 1          //Pokemon name
           + sizeof(uint32_t)                                   //Pos x
           + sizeof(uint32_t);                                  //Pos y
}

t_serialization_information* serialize(t_request* request){

    t_serializable_object* serializable_object = serializable_object_with_code(request -> operation);
    return (*(serializable_object -> serialize_function)) (request -> structure);
}

t_serialization_information* serialize_new_pokemon(void* structure){

    t_new_pokemon* new_pokemon = (t_new_pokemon*) structure;
    uint32_t amount_of_bytes_of_new_pokemon = amount_of_bytes_of_new(new_pokemon);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                    // operation
            + sizeof(uint32_t)                  // structure size
            + amount_of_bytes_of_new_pokemon;   // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = NEW_POKEMON;
    uint32_t pokemon_name_length = strlen(new_pokemon -> pokemon_name) + 1;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_new_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &pokemon_name_length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, new_pokemon -> pokemon_name, pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(serialized_request + offset, &(new_pokemon -> pos_x), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(new_pokemon -> pos_y), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(new_pokemon -> quantity), sizeof(uint32_t));

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
}

t_serialization_information* serialize_appeared_pokemon(void* structure){

    t_appeared_pokemon* appeared_pokemon = (t_appeared_pokemon*) structure;
    uint32_t amount_of_bytes_of_appeared_pokemon = amount_of_bytes_of_appeared(appeared_pokemon);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                        // operation
            + sizeof(uint32_t)                      // structure size
            + amount_of_bytes_of_appeared_pokemon;  // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = APPEARED_POKEMON;
    uint32_t pokemon_name_length = strlen(appeared_pokemon -> pokemon_name) + 1;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_appeared_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &pokemon_name_length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, appeared_pokemon -> pokemon_name, pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(serialized_request + offset, &(appeared_pokemon -> pos_x), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(appeared_pokemon -> pos_y), sizeof(uint32_t));

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
}

t_serialization_information* serialize_get_pokemon(void* structure){

    t_get_pokemon* get_pokemon = (t_get_pokemon*) structure;
    uint32_t amount_of_bytes_of_get_pokemon = amount_of_bytes_of_get(get_pokemon);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                        // operation
            + sizeof(uint32_t)                      // structure size
            + amount_of_bytes_of_get_pokemon;  		// structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = GET_POKEMON;
    uint32_t pokemon_name_length = strlen(get_pokemon -> pokemon_name) + 1;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_get_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &pokemon_name_length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, get_pokemon -> pokemon_name, pokemon_name_length);

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
}

t_serialization_information* serialize_localized_pokemon(void* structure) {

    t_localized_pokemon* localized_pokemon = (t_localized_pokemon*) structure;
    uint32_t amount_of_bytes_of_localized_pokemon = amount_of_bytes_of_localized(localized_pokemon);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                        // operation
            + sizeof(uint32_t)                      // structure size
            + amount_of_bytes_of_localized_pokemon;  // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = LOCALIZED_POKEMON;
    uint32_t pokemon_name_length = strlen(localized_pokemon -> pokemon_name) + 1;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_localized_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &pokemon_name_length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, localized_pokemon -> pokemon_name, pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(serialized_request + offset, &(localized_pokemon -> quantity), sizeof(uint32_t));
    offset += sizeof(uint32_t);

    for(int i = 0; i < list_size(localized_pokemon -> positions); i++){
        uint32_t* position = (uint32_t* )list_get(localized_pokemon -> positions, i);
        memcpy(serialized_request + offset, position, sizeof(uint32_t));
        offset += sizeof(uint32_t);
    }

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;

    return serialization_information;
}

t_serialization_information* serialize_catch_pokemon(void* structure){

    t_catch_pokemon* catch_pokemon = (t_catch_pokemon*) structure;
    uint32_t amount_of_bytes_of_catch_pokemon = amount_of_bytes_of_catch(catch_pokemon);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                        // operation
            + sizeof(uint32_t)                      // structure size
            + amount_of_bytes_of_catch_pokemon;     // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = CATCH_POKEMON;
    uint32_t pokemon_name_length = strlen(catch_pokemon -> pokemon_name) + 1;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_catch_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &pokemon_name_length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, catch_pokemon -> pokemon_name, pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(serialized_request + offset, &(catch_pokemon -> pos_x), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(catch_pokemon -> pos_y), sizeof(uint32_t));

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
}

t_serialization_information* serialize_caught_pokemon(void* structure){

    t_caught_pokemon* caught_pokemon = (t_caught_pokemon*) structure;
    uint32_t amount_of_bytes_of_caught_pokemon = sizeof(uint32_t); //caught_status
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                        // operation
            + sizeof(uint32_t)                      // structure size
            + amount_of_bytes_of_caught_pokemon;    // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = CAUGHT_POKEMON;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_caught_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(caught_pokemon -> caught_status), sizeof(uint32_t));

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
}

t_serialization_information* serialize_subscribe_me(void* structure){

    t_subscribe_me* subscribe_me = (t_subscribe_me*) structure;
    uint32_t amount_of_bytes_of_queue = sizeof(uint32_t);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                            // operation
            + sizeof(uint32_t)                          // structure size
            + amount_of_bytes_of_queue;                 // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = SUBSCRIBE_ME;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_queue, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(subscribe_me -> operation_queue), sizeof(uint32_t));

    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    return serialization_information;
}

t_serialization_information* serialize_identified_message(void* structure){
    t_identified_message* identified_message = (t_identified_message*) structure;

    t_serialization_information* request_serialization_information = serialize(identified_message -> request);

    uint32_t amount_of_bytes_of_identified_message =
            sizeof(uint32_t)                                        // message_id
            + sizeof(uint32_t)                                      // serialization_information_amount_of_bytes
            + request_serialization_information -> amount_of_bytes;

    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                            // operation
            + sizeof(uint32_t)                          // structure size
            + amount_of_bytes_of_identified_message;    // structure

    void* serialized_request = safe_malloc(amount_of_bytes_of_request);

    uint32_t operation = IDENTIFIED_MESSAGE;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_identified_message, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(identified_message -> message_id), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(request_serialization_information -> amount_of_bytes), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, request_serialization_information -> serialized_request, request_serialization_information -> amount_of_bytes);

    free_serialization_information(request_serialization_information);
    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    serialization_information -> serialized_request = serialized_request;

    return serialization_information;
}