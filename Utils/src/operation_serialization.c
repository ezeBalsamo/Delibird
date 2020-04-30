#include "../include/operation_serialization.h"
#include "../include/processes_information.h"
#include <string.h>
#include <stdlib.h>

uint32_t amount_of_bytes_of_appeared(char** arguments){
    return sizeof(uint32_t)                 //Pokemon name length
            + strlen(arguments[0]) + 1   //Pokemon name
            + sizeof(uint32_t)              //Pos x
            + sizeof(uint32_t)              //Pos y
            + sizeof(uint32_t);             //id_mensaje
}

uint32_t amount_of_bytes_of_new(char** arguments){
    return sizeof(uint32_t)                 //Pokemon name length
            + strlen(arguments[0]) + 1   //Pokemon name
            + sizeof(uint32_t)              //Pos x
            + sizeof(uint32_t)              //Pos y
            + sizeof(uint32_t)              //Cantidad
            + sizeof(uint32_t);             //id_mensaje
}

uint32_t amount_of_bytes_of_catch(char** arguments){
    return sizeof(uint32_t)                 //Pokemon name length
           + strlen(arguments[0]) + 1    //Pokemon name
           + sizeof(uint32_t)               //Pos x
           + sizeof(uint32_t)               //Pos y
           + sizeof(uint32_t);              //id_mensaje
}

uint32_t amount_of_bytes_of_get(char** arguments){
    return sizeof(uint32_t)                 //Pokemon name length
           + strlen(arguments[0]) + 1;    	//Pokemon name
}

uint32_t amount_of_bytes_of_caught(){
    return sizeof(uint32_t)                 //id_mensaje
           + sizeof(uint32_t);              //acknowledgment
}

t_serialization_information* serialize(t_request* request){

    t_operation_information* operation_information = operation_information_with_code(request -> operation);
    return (*(operation_information -> serialize_function)) ((char**) request -> structure);
}

t_serialization_information* serialize_appeared_pokemon(char** arguments){

    uint32_t amount_of_bytes_of_appeared_pokemon = amount_of_bytes_of_appeared(arguments);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                        // operation
            + sizeof(uint32_t)                      // structure size
            + amount_of_bytes_of_appeared_pokemon;  // structure
    uint32_t amount_of_bytes =
            sizeof(uint32_t)                        // total amount (used by socket)
            + amount_of_bytes_of_request;

    void* serialized_request = malloc(amount_of_bytes);

    uint32_t operation = APPEARED_POKEMON;
    uint32_t pokemon_name_length = strlen(arguments[0]) + 1;
    uint32_t pos_x = atoi(arguments[1]);
    uint32_t pos_y = atoi(arguments[2]);
    uint32_t message_id = arguments[3] == NULL?-1:(atoi(arguments[3]));

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_appeared_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &pokemon_name_length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, arguments[0], pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(serialized_request + offset, &pos_x, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &pos_y, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &message_id, sizeof(uint32_t));

    t_serialization_information* serialization_information = malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes;
    return serialization_information;
}

t_serialization_information* serialize_new_pokemon(char** arguments){

    uint32_t amount_of_bytes_of_new_pokemon = amount_of_bytes_of_new(arguments);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                    // operation
            + sizeof(uint32_t)                  // structure size
            + amount_of_bytes_of_new_pokemon;   // structure
    uint32_t amount_of_bytes =
            sizeof(uint32_t)                    // total amount (used by socket)
            + amount_of_bytes_of_request;

    void* serialized_request = malloc(amount_of_bytes);

    uint32_t operation = NEW_POKEMON;
    uint32_t pokemon_name_length = strlen(arguments[0]) + 1;
    uint32_t pos_x = atoi(arguments[1]);
    uint32_t pos_y = atoi(arguments[2]);
    uint32_t quantity = atoi(arguments[3]);
    uint32_t message_id = arguments[4] == NULL?-1:(atoi(arguments[4]));

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_new_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &pokemon_name_length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, arguments[0], pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(serialized_request + offset, &pos_x, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &pos_y, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &quantity, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &message_id, sizeof(uint32_t));

    t_serialization_information* serialization_information = malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes;
    return serialization_information;
}

t_serialization_information* serialize_catch_pokemon(char** arguments){

    uint32_t amount_of_bytes_of_catch_pokemon = amount_of_bytes_of_catch(arguments);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                        // operation
            + sizeof(uint32_t)                      // structure size
            + amount_of_bytes_of_catch_pokemon;     // structure
    uint32_t amount_of_bytes =
            sizeof(uint32_t)                        // total amount (used by socket)
            + amount_of_bytes_of_request;

    void* serialized_request = malloc(amount_of_bytes);

    uint32_t operation = CATCH_POKEMON;
    uint32_t pokemon_name_length = strlen(arguments[0]) + 1;
    uint32_t pos_x = atoi(arguments[1]);
    uint32_t pos_y = atoi(arguments[2]);
    uint32_t message_id = arguments[3] == NULL?-1:(atoi(arguments[3]));

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_catch_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &pokemon_name_length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, arguments[0], pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(serialized_request + offset, &pos_x, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &pos_y, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &message_id, sizeof(uint32_t));

    t_serialization_information* serialization_information = malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes;
    return serialization_information;
}

t_serialization_information* serialize_get_pokemon(char** arguments){

    uint32_t amount_of_bytes_of_get_pokemon = amount_of_bytes_of_get(arguments);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                        // operation
            + sizeof(uint32_t)                      // structure size
            + amount_of_bytes_of_get_pokemon;  		// structure
    uint32_t amount_of_bytes =
            sizeof(uint32_t)                        // total amount (used by socket)
            + amount_of_bytes_of_request;

    void* serialized_request = malloc(amount_of_bytes);

    uint32_t operation = GET_POKEMON;
    uint32_t pokemon_name_length = strlen(arguments[0]) + 1;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_get_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &pokemon_name_length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, arguments[0], pokemon_name_length);

    t_serialization_information* serialization_information = malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes;
    return serialization_information;
}

t_serialization_information* serialize_caught_pokemon(char** arguments){

    uint32_t amount_of_bytes_of_caught_pokemon = amount_of_bytes_of_caught();
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                        // operation
            + sizeof(uint32_t)                      // structure size
            + amount_of_bytes_of_caught_pokemon;  		// structure
    uint32_t amount_of_bytes =
            sizeof(uint32_t)                        // total amount (used by socket)
            + amount_of_bytes_of_request;

    void* serialized_request = malloc(amount_of_bytes);

    uint32_t operation = CAUGHT_POKEMON;
    uint32_t message_id = atoi(arguments[0]);
    uint32_t acknowledgement = atoi(arguments[1]);

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_caught_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &message_id, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &acknowledgement, sizeof(uint32_t));

    t_serialization_information* serialization_information = malloc(sizeof(t_serialization_information));
    serialization_information -> serialized_request = serialized_request;
    serialization_information -> amount_of_bytes = amount_of_bytes;
    return serialization_information;
}