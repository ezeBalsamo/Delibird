#include "../include/operation_deserialization.h"
#include "../include/operations_information.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

    printf("request deserialized!\n");
    printf("request operation number: %d\n", APPEARED_POKEMON);
    printf("request structure: pokemon: %s, pos x: %d, pos y: %d, id_mensaje: %d.\n", pokemon_name, pos_x, pos_y, message_id);
}

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

    printf("request deserialized!\n");
    printf("request operation number: %d\n", NEW_POKEMON);
    printf("request structure: pokemon: %s, pos x: %d, pos y: %d, quantity: %d, id_mensaje: %d.\n", pokemon_name, pos_x, pos_y, quantity, message_id);
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

    printf("request deserialized!\n");
    printf("request operation number: %d\n", CATCH_POKEMON);
    printf("request structure: pokemon: %s, pos x: %d, pos y: %d, id_mensaje: %d.\n", pokemon_name, pos_x, pos_y, message_id);
}

t_request* deserialize_caught_pokemon(void* serialized_structure){

    uint32_t message_id;
    uint32_t caught_status;    // 0/1 - FAIL/OK
    uint32_t offset = 0;

    memcpy(&message_id, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&caught_status, serialized_structure + offset, sizeof(uint32_t));

    printf("request deserialized!\n");
    printf("request operation number: %d\n", CAUGHT_POKEMON);
    printf("request structure: : id_mensaje: %d,  caught_status: %d.\n", message_id,  caught_status);
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
    offset += pokemon_name_length;

    printf("request deserialized!\n");
    printf("request operation number: %d\n", GET_POKEMON);
    printf("request structure: pokemon: %s.\n", pokemon_name);
}


t_request* deserialize_suscribe_me(void* serialized_structure){
    uint32_t operation_queue;

    memcpy(&operation_queue, serialized_structure, sizeof(uint32_t));

    printf("request deserialized!\n");
    printf("request operation number: %d\n", SUSCRIBE_ME);
    printf("operation queue: %d\n", operation_queue);
}

t_request* deserialize_localized_pokemon(void* serialized_structure){
    //TODO: implementar deserializacion de LOCALIZED
};