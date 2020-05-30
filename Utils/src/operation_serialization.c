#include "../include/operation_serialization.h"
#include "../include/operations_information.h"
#include <string.h>
#include <stdlib.h>

uint32_t amount_of_bytes_of_appeared(t_appeared_pokemon* appeared_pokemon){
    return sizeof(uint32_t)                                     //Pokemon name length
            + strlen(appeared_pokemon -> pokemon_name) + 1      //Pokemon name
            + sizeof(uint32_t)                                  //Pos x
            + sizeof(uint32_t)                                  //Pos y
            + sizeof(uint32_t);                                 //message_id
}
uint32_t amount_of_bytes_of_localized(t_localized_pokemon * localized_pokemon){
    return sizeof(uint32_t)                                   //Pokemon name length
           + strlen(localized_pokemon -> pokemon_name) + 1    //Pokemon name
           + sizeof(uint32_t)                                 //quantity
           + sizeof(list_size(localized_pokemon->positions)); //positions
}
uint32_t amount_of_bytes_of_new(t_new_pokemon* new_pokemon){
    return sizeof(uint32_t)                                     //Pokemon name length
            + strlen(new_pokemon -> pokemon_name) + 1           //Pokemon name
            + sizeof(uint32_t)                                  //Pos x
            + sizeof(uint32_t)                                  //Pos y
            + sizeof(uint32_t)                                  //Cantidad
            + sizeof(uint32_t);                                 //message_id
}

uint32_t amount_of_bytes_of_catch(t_catch_pokemon* catch_pokemon){
    return sizeof(uint32_t)                                     //Pokemon name length
           + strlen(catch_pokemon -> pokemon_name) + 1          //Pokemon name
           + sizeof(uint32_t)                                   //Pos x
           + sizeof(uint32_t)                                   //Pos y
           + sizeof(uint32_t);                                  //message_id
}

uint32_t amount_of_bytes_of_get(t_get_pokemon* get_pokemon){
    return sizeof(uint32_t)                                     //Pokemon name length
           + strlen(get_pokemon -> pokemon_name) + 1    	    //Pokemon name
           + sizeof(uint32_t);                                  //message_id
}

uint32_t amount_of_bytes_of_caught(){
    return sizeof(uint32_t)                                     //message_id
           + sizeof(uint32_t);                                  //caught_status
}

t_serialization_information* serialize(t_request* request){

    t_operation_information* operation_information = operation_information_with_code(request -> operation);
    return (*(operation_information -> serialize_function)) (request -> structure);
}

t_serialization_information* serialize_appeared_pokemon(void* structure){

    t_appeared_pokemon* appeared_pokemon = (t_appeared_pokemon*) structure;
    uint32_t amount_of_bytes_of_appeared_pokemon = amount_of_bytes_of_appeared(appeared_pokemon);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                        // operation
            + sizeof(uint32_t)                      // structure size
            + amount_of_bytes_of_appeared_pokemon;  // structure
    uint32_t total_amount_of_bytes =
            sizeof(uint32_t)                        // total amount (used by socket)
            + amount_of_bytes_of_request;

    void* serialized_request = malloc(total_amount_of_bytes);

    uint32_t operation = APPEARED_POKEMON;
    uint32_t pokemon_name_length = strlen(appeared_pokemon -> pokemon_name) + 1;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
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
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(appeared_pokemon -> message_id), sizeof(uint32_t));

    t_serialization_information* serialization_information = malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = total_amount_of_bytes;
    return serialization_information;
}

t_serialization_information* serialize_new_pokemon(void* structure){

    t_new_pokemon* new_pokemon = (t_new_pokemon*) structure;
    uint32_t amount_of_bytes_of_new_pokemon = amount_of_bytes_of_new(new_pokemon);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                    // operation
            + sizeof(uint32_t)                  // structure size
            + amount_of_bytes_of_new_pokemon;   // structure
    uint32_t total_amount_of_bytes =
            sizeof(uint32_t)                    // total amount (used by socket)
            + amount_of_bytes_of_request;

    void* serialized_request = malloc(total_amount_of_bytes);

    uint32_t operation = NEW_POKEMON;
    uint32_t pokemon_name_length = strlen(new_pokemon -> pokemon_name) + 1;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
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
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(new_pokemon -> message_id), sizeof(uint32_t));

    t_serialization_information* serialization_information = malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = total_amount_of_bytes;
    return serialization_information;
}

t_serialization_information* serialize_catch_pokemon(void* structure){

    t_catch_pokemon* catch_pokemon = (t_catch_pokemon*) structure;
    uint32_t amount_of_bytes_of_catch_pokemon = amount_of_bytes_of_catch(catch_pokemon);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                        // operation
            + sizeof(uint32_t)                      // structure size
            + amount_of_bytes_of_catch_pokemon;     // structure
    uint32_t total_amount_of_bytes =
            sizeof(uint32_t)                        // total amount (used by socket)
            + amount_of_bytes_of_request;

    void* serialized_request = malloc(total_amount_of_bytes);

    uint32_t operation = CATCH_POKEMON;
    uint32_t pokemon_name_length = strlen(catch_pokemon -> pokemon_name) + 1;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
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
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(catch_pokemon -> message_id), sizeof(uint32_t));

    t_serialization_information* serialization_information = malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = total_amount_of_bytes;
    return serialization_information;
}

t_serialization_information* serialize_get_pokemon(void* structure){

    t_get_pokemon* get_pokemon = (t_get_pokemon*) structure;
    uint32_t amount_of_bytes_of_get_pokemon = amount_of_bytes_of_get(get_pokemon);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                        // operation
            + sizeof(uint32_t)                      // structure size
            + amount_of_bytes_of_get_pokemon;  		// structure
    uint32_t total_amount_of_bytes =
            sizeof(uint32_t)                        // total amount (used by socket)
            + amount_of_bytes_of_request;

    void* serialized_request = malloc(total_amount_of_bytes);

    uint32_t operation = GET_POKEMON;
    uint32_t pokemon_name_length = strlen(get_pokemon -> pokemon_name) + 1;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_get_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &pokemon_name_length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, get_pokemon -> pokemon_name, pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(serialized_request + offset, &(get_pokemon -> message_id), sizeof(uint32_t));

    t_serialization_information* serialization_information = malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = total_amount_of_bytes;
    return serialization_information;
}

t_serialization_information* serialize_caught_pokemon(void* structure){

    t_caught_pokemon* caught_pokemon = (t_caught_pokemon*) structure;
    uint32_t amount_of_bytes_of_caught_pokemon = amount_of_bytes_of_caught();
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                        // operation
            + sizeof(uint32_t)                      // structure size
            + amount_of_bytes_of_caught_pokemon;    // structure
    uint32_t total_amount_of_bytes =
            sizeof(uint32_t)                        // total amount (used by socket)
            + amount_of_bytes_of_request;

    void* serialized_request = malloc(total_amount_of_bytes);

    uint32_t operation = CAUGHT_POKEMON;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_caught_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(caught_pokemon -> message_id), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(caught_pokemon -> caught_status), sizeof(uint32_t));

    t_serialization_information* serialization_information = malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = total_amount_of_bytes;
    return serialization_information;
}

t_serialization_information* serialize_subscribe_me(void* structure){

    t_subscribe_me* subscribe_me = (t_subscribe_me*) structure;
    uint32_t amount_of_bytes_of_queue = sizeof(uint32_t);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                        // operation
            + sizeof(uint32_t)                      // structure size
            + amount_of_bytes_of_queue;             // structure
    uint32_t total_amount_of_bytes =
            sizeof(uint32_t)                        // total amount (used by socket)
            + amount_of_bytes_of_request;

    void* serialized_request = malloc(total_amount_of_bytes);

    uint32_t operation = SUBSCRIBE_ME;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_queue, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &(subscribe_me -> operation_queue), sizeof(uint32_t));

    t_serialization_information* serialization_information = malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = total_amount_of_bytes;
    return serialization_information;
}

t_serialization_information* serialize_localized_pokemon(void* structure) {

    t_localized_pokemon *localized_pokemon = (t_localized_pokemon *) structure;
    uint32_t amount_of_bytes_of_localized_pokemon = amount_of_bytes_of_localized(localized_pokemon);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                        // operation
            + sizeof(uint32_t)                      // structure size
            + amount_of_bytes_of_localized_pokemon;  // structure
    uint32_t total_amount_of_bytes =
            sizeof(uint32_t)                        // total amount (used by socket)
            + amount_of_bytes_of_request;

    void *serialized_request = malloc(total_amount_of_bytes);

    uint32_t operation = LOCALIZED_POKEMON;
    uint32_t pokemon_name_length = strlen(localized_pokemon->pokemon_name) + 1;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_localized_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &pokemon_name_length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, localized_pokemon->pokemon_name, pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(serialized_request + offset, &(localized_pokemon->quantity), sizeof(uint32_t));
    offset += sizeof(uint32_t);

    for(int i=0;i<list_size(localized_pokemon->positions);i++){
        uint32_t pos = list_get(localized_pokemon->positions,i);

        memcpy(serialized_request + offset,&pos, sizeof(uint32_t));
        offset += sizeof(uint32_t);
    }
    memcpy(serialized_request + offset, &(localized_pokemon->positions), sizeof(t_list *));

    t_serialization_information *serialization_information = malloc(sizeof(t_serialization_information));
    serialization_information->serialized_request = serialized_request;
    serialization_information->amount_of_bytes = total_amount_of_bytes;

    return serialization_information;
}