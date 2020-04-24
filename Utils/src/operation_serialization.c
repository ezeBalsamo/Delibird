#include "../include/operation_serialization.h"
#include <string.h>
#include <stdlib.h>

uint32_t amount_of_bytes_of_appeared(appearedPokemonArguments* arguments){
    return   sizeof(uint32_t)                        //Pokemon name length
             + strlen(arguments -> pokemon) + 1        //Pokemon name
             + sizeof(uint32_t)                        //posx
             + sizeof(uint32_t)                        //posy
             + sizeof(uint32_t);                        //id_mensaje
}

void serialize_appeared_pokemon(appearedPokemonArguments* arguments, void** serialized_structure, uint32_t* amount_of_bytes){

    uint32_t amount_of_bytes_of_appeared_pokemon = amount_of_bytes_of_appeared(arguments);
    uint32_t amount_of_bytes_of_request =
            + sizeof(uint32_t)              // operation
            + sizeof(uint32_t)              // structure size
            + amount_of_bytes_of_appeared_pokemon;    // structure
    *amount_of_bytes =
            sizeof(uint32_t)                // total amount (used by socket)
            + amount_of_bytes_of_request;

    *serialized_structure = malloc(*amount_of_bytes);

    uint32_t operation = APPEARED_POKEMON;
    uint32_t pokemon_name_length = strlen(arguments -> pokemon) + 1;

    uint32_t offset = 0;

    memcpy(*serialized_structure + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &amount_of_bytes_of_appeared_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &pokemon_name_length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, arguments->pokemon, pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(*serialized_structure + offset, &arguments->posx, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &arguments->posy, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &arguments->id_mensaje, sizeof(uint32_t));
}

void serialize(request* request, void** serialized_request, uint32_t* amount_of_bytes){
    uint32_t operation = request -> operation;

    switch (operation){
        case APPEARED_POKEMON:{
            serialize_appeared_pokemon((appearedPokemonArguments*) request -> structure, serialized_request, amount_of_bytes);
            break;
        };
    }
}

void* deserialize(void* serialized_request){}

request* deserialize_appeared_pokemon(void* serialized_structure) {return NULL;}
