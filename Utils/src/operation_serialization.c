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

void serialize(t_request* request, void** serialized_request, uint32_t* amount_of_bytes){

    t_operation_information* operation_information = operation_information_with_code(request -> operation);
    (*(operation_information -> serialize_function)) ((char**) request -> structure, serialized_request, amount_of_bytes);
}

void serialize_appeared_pokemon(char** arguments, void** serialized_structure, uint32_t* amount_of_bytes){

    uint32_t amount_of_bytes_of_appeared_pokemon = amount_of_bytes_of_appeared(arguments);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                        // operation
            + sizeof(uint32_t)                      // structure size
            + amount_of_bytes_of_appeared_pokemon;  // structure
    *amount_of_bytes =
            sizeof(uint32_t)                        // total amount (used by socket)
            + amount_of_bytes_of_request;

    *serialized_structure = malloc(*amount_of_bytes);

    uint32_t operation = APPEARED_POKEMON;
    uint32_t pokemon_name_length = strlen(arguments[0]) + 1;
    uint32_t pos_x = atoi(arguments[1]);
    uint32_t pos_y = atoi(arguments[2]);
    uint32_t message_id = arguments[3] == NULL?-1:(atoi(arguments[3]));

    uint32_t offset = 0;

    memcpy(*serialized_structure + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &amount_of_bytes_of_appeared_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &pokemon_name_length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, arguments[0], pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(*serialized_structure + offset, &pos_x, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &pos_y, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &message_id, sizeof(uint32_t));
}

void serialize_new_pokemon(char** arguments, void** serialized_structure, uint32_t* amount_of_bytes){

    uint32_t amount_of_bytes_of_new_pokemon = amount_of_bytes_of_new(arguments);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                    // operation
            + sizeof(uint32_t)                  // structure size
            + amount_of_bytes_of_new_pokemon;   // structure
    *amount_of_bytes =
            sizeof(uint32_t)                    // total amount (used by socket)
            + amount_of_bytes_of_request;

    *serialized_structure = malloc(*amount_of_bytes);

    uint32_t operation = NEW_POKEMON;
    uint32_t pokemon_name_length = strlen(arguments[0]) + 1;
    uint32_t pos_x = atoi(arguments[1]);
    uint32_t pos_y = atoi(arguments[2]);
    uint32_t quantity = atoi(arguments[3]);
    uint32_t message_id = arguments[4] == NULL?-1:(atoi(arguments[4]));

    uint32_t offset = 0;

    memcpy(*serialized_structure + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &amount_of_bytes_of_new_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &pokemon_name_length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, arguments[0], pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(*serialized_structure + offset, &pos_x, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &pos_y, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &quantity, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &message_id, sizeof(uint32_t));
}

void serialize_catch_pokemon(char** arguments, void** serialized_structure, uint32_t* amount_of_bytes){

    uint32_t amount_of_bytes_of_catch_pokemon = amount_of_bytes_of_catch(arguments);
    uint32_t amount_of_bytes_of_request =
            sizeof(uint32_t)                        // operation
            + sizeof(uint32_t)                      // structure size
            + amount_of_bytes_of_catch_pokemon;     // structure
    *amount_of_bytes =
            sizeof(uint32_t)                        // total amount (used by socket)
            + amount_of_bytes_of_request;

    *serialized_structure = malloc(*amount_of_bytes);

    uint32_t operation = CATCH_POKEMON;
    uint32_t pokemon_name_length = strlen(arguments[0]) + 1;
    uint32_t pos_x = atoi(arguments[1]);
    uint32_t pos_y = atoi(arguments[2]);
    uint32_t message_id = arguments[3] == NULL?-1:(atoi(arguments[3]));

    uint32_t offset = 0;

    memcpy(*serialized_structure + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &amount_of_bytes_of_catch_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &pokemon_name_length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, arguments[0], pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(*serialized_structure + offset, &pos_x, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &pos_y, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &message_id, sizeof(uint32_t));
}