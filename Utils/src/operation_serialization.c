#include "../include/operation_serialization.h"
#include <string.h>
#include <stdlib.h>

uint32_t amount_of_bytes_of_appeared(char** arguments){
    return sizeof(uint32_t)                 //Pokemon name length
            + strlen(arguments[0]) + 1   //Pokemon name
            + sizeof(uint32_t)              //Pos x
            + sizeof(uint32_t)              //Pos y
            + sizeof(uint32_t);             //id_mensaje
}

void serialize_new_pokemon(char** arguments, void** serialized_structure, uint32_t* amount_of_bytes){}

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
    uint32_t id_mensaje = arguments[3] == NULL?-1:(atoi(arguments[3]));

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
    memcpy(*serialized_structure + offset, &id_mensaje, sizeof(uint32_t));
}


uint32_t amount_of_bytes_of_new(newPokemonArguments* arguments){
    return   sizeof(uint32_t)                        //Pokemon name length
             + strlen(arguments -> pokemon) + 1        //Pokemon name
             + sizeof(uint32_t)                        //posx
             + sizeof(uint32_t)                        //posy
             + sizeof(uint32_t)                        //cantidad
             + sizeof(uint32_t);                        //id_mensaje
}

void serialize_new_pokemon(newPokemonArguments* arguments, void** serialized_structure, uint32_t* amount_of_bytes){

    uint32_t amount_of_bytes_of_new_pokemon = amount_of_bytes_of_new(arguments);
    uint32_t amount_of_bytes_of_request =
            + sizeof(uint32_t)              // operation
            + sizeof(uint32_t)              // structure size
            + amount_of_bytes_of_new_pokemon;    // structure
    *amount_of_bytes =
            sizeof(uint32_t)                // total amount (used by socket)
            + amount_of_bytes_of_request;

    *serialized_structure = malloc(*amount_of_bytes);

    uint32_t operation = NEW_POKEMON;
    uint32_t pokemon_name_length = strlen(arguments -> pokemon) + 1;

    uint32_t offset = 0;

    memcpy(*serialized_structure + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &amount_of_bytes_of_new_pokemon, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &pokemon_name_length, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, arguments->pokemon, pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(*serialized_structure + offset, &arguments->posx, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &arguments->posy, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &arguments->cantidad, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &arguments->id_mensaje, sizeof(uint32_t));
}

uint32_t amount_of_bytes_of_catch(catchPokemonArguments* arguments){
    return   sizeof(uint32_t)                        //Pokemon name length
             + strlen(arguments -> pokemon) + 1        //Pokemon name
             + sizeof(uint32_t)                        //posx
             + sizeof(uint32_t)                        //posy
             + sizeof(uint32_t);                        //id_mensaje
}

void serialize_catch_pokemon(catchPokemonArguments* arguments, void** serialized_structure, uint32_t* amount_of_bytes){

    uint32_t amount_of_bytes_of_catch_pokemon = amount_of_bytes_of_catch(arguments);
    uint32_t amount_of_bytes_of_request =
            + sizeof(uint32_t)              // operation
            + sizeof(uint32_t)              // structure size
            + amount_of_bytes_of_catch_pokemon;    // structure
    *amount_of_bytes =
            sizeof(uint32_t)                // total amount (used by socket)
            + amount_of_bytes_of_request;

    *serialized_structure = malloc(*amount_of_bytes);

    uint32_t operation = CATCH_POKEMON;
    uint32_t pokemon_name_length = strlen(arguments -> pokemon) + 1;

    uint32_t offset = 0;

    memcpy(*serialized_structure + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(*serialized_structure + offset, &amount_of_bytes_of_catch_pokemon, sizeof(uint32_t));
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

request* deserialize_appeared_pokemon(void* serialized_structure) {

    request* request = malloc(sizeof(request));
    request -> operation = APPEARED_POKEMON;

    appearedPokemonArguments* arguments = malloc(sizeof(appearedPokemonArguments));
    uint32_t pokemon_name_length;

    uint32_t offset = 0;

    memcpy(&pokemon_name_length, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    arguments -> pokemon = malloc(pokemon_name_length);
    memcpy(arguments -> pokemon, serialized_structure + offset, pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(&(arguments -> posx), serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&(arguments -> posy), serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&(arguments -> id_mensaje), serialized_structure + offset, sizeof(uint32_t));

    printf("request deserialized!\n");
    printf("request operation number: %d\n", request->operation);
    printf("request structure: pokemon %s, %d posx, %d posy, %d id_mensaje.\n", arguments -> pokemon, arguments -> posx, arguments -> posy, arguments -> id_mensaje);

    request -> structure = (void*) arguments;

    return request;
}

request* deserialize_new_pokemon(void* serialized_structure) {

void serialize(t_request* request, void** serialized_request, uint32_t* amount_of_bytes){
    request* request = malloc(sizeof(request));
    request -> operation = NEW_POKEMON;

    t_operation_information* operation_information = operation_information_with_code(request -> operation);
    (*(operation_information -> serialize_function)) ((char**) request -> structure, serialized_request, amount_of_bytes);
}
    newPokemonArguments* arguments = malloc(sizeof(newPokemonArguments));
    uint32_t pokemon_name_length;

    uint32_t offset = 0;

    memcpy(&pokemon_name_length, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    arguments -> pokemon = malloc(pokemon_name_length);
    memcpy(arguments -> pokemon, serialized_structure + offset, pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(&(arguments -> posx), serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&(arguments -> posy), serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&(arguments -> cantidad), serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&(arguments -> id_mensaje), serialized_structure + offset, sizeof(uint32_t));

    printf("request deserialized!\n");
    printf("request operation number: %d\n", request->operation);
    printf("request structure: pokemon %s, %d posx, %d posy, %d cantidad, %d id_mensaje.\n", arguments -> pokemon, arguments -> posx, arguments -> posy, arguments -> cantidad, arguments -> id_mensaje);

    request -> structure = (void*) arguments;

    return request;
}

request* deserialize_catch_pokemon(void* serialized_structure) {

    request* request = malloc(sizeof(request));
    request -> operation = CATCH_POKEMON;

    catchPokemonArguments* arguments = malloc(sizeof(catchPokemonArguments));
    uint32_t pokemon_name_length;

    uint32_t offset = 0;

    memcpy(&pokemon_name_length, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    arguments -> pokemon = malloc(pokemon_name_length);
    memcpy(arguments -> pokemon, serialized_structure + offset, pokemon_name_length);
    offset += pokemon_name_length;
    memcpy(&(arguments -> posx), serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&(arguments -> posy), serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&(arguments -> id_mensaje), serialized_structure + offset, sizeof(uint32_t));

    printf("request deserialized!\n");
    printf("request operation number: %d\n", request->operation);
    printf("request structure: pokemon %s, %d posx, %d posy, %d id_mensaje.\n", arguments -> pokemon, arguments -> posx, arguments -> posy, arguments -> id_mensaje);

    request -> structure = (void*) arguments;

    return request;
}