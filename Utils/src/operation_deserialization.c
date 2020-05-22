#include "../include/operation_deserialization.h"
#include <stdlib.h>
#include <string.h>

t_printable_object* deserialize_appeared_pokemon(void* serialized_structure) {

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

    t_printable_object* printable_object = malloc(sizeof(t_printable_object));
    printable_object -> object = (void*) appeared_pokemon;
    printable_object -> print_function = (char *(*)(void *)) appeared_pokemon_as_string;
    return printable_object;
}

t_printable_object* deserialize_new_pokemon(void* serialized_structure) {

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

    t_printable_object* printable_object = malloc(sizeof(t_printable_object));
    printable_object -> object = (void*) new_pokemon;
    printable_object -> print_function = (char *(*)(void *)) new_pokemon_as_string;
    return printable_object;
}

t_printable_object* deserialize_catch_pokemon(void* serialized_structure) {

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

    t_printable_object* printable_object = malloc(sizeof(t_printable_object));
    printable_object -> object = (void*) catch_pokemon;
    printable_object -> print_function = (char *(*)(void *)) catch_pokemon_as_string;
    return printable_object;
}

t_printable_object* deserialize_caught_pokemon(void* serialized_structure){

    uint32_t message_id;
    uint32_t caught_status;    // 0/1 - FAIL/OK
    uint32_t offset = 0;

    memcpy(&message_id, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&caught_status, serialized_structure + offset, sizeof(uint32_t));

    t_caught_pokemon* caught_pokemon = malloc(sizeof(t_catch_pokemon));
    caught_pokemon -> message_id = message_id;
    caught_pokemon->caught_status = caught_status;

    t_printable_object* printable_object = malloc(sizeof(t_printable_object));
    printable_object -> object = (void*) caught_pokemon;
    printable_object -> print_function = (char *(*)(void *)) caught_pokemon_as_string;
    return printable_object;
}

t_printable_object* deserialize_get_pokemon(void* serialized_structure){
    //Mensaje GET envia unicamente el pokemon
    uint32_t pokemon_name_length;
    char* pokemon_name;
    uint32_t offset = 0;

    memcpy(&pokemon_name_length, serialized_structure + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    pokemon_name = malloc(pokemon_name_length);
    memcpy(pokemon_name, serialized_structure + offset, pokemon_name_length);

    t_get_pokemon* get_pokemon = malloc(sizeof(t_get_pokemon));
    get_pokemon->pokemon_name = pokemon_name;

    t_printable_object* printable_object = malloc(sizeof(t_printable_object));
    printable_object -> object = (void*) get_pokemon;
    printable_object -> print_function = (char *(*)(void *)) get_pokemon_as_string;

    return printable_object;
}

t_printable_object* deserialize_subscribe_me(void* serialized_structure){

    uint32_t* operation_queue = malloc(sizeof(uint32_t));

    memcpy(operation_queue, serialized_structure, sizeof(uint32_t));

    t_subscribe_me* subscribe_me = malloc(sizeof(t_subscribe_me));
    subscribe_me -> operation_queue = *operation_queue;


    t_printable_object* printable_object = malloc(sizeof(t_printable_object));
    printable_object -> object = (void*) subscribe_me;
    printable_object -> print_function = (char *(*)(void *)) subscribe_me_as_string;
    return printable_object;

}

t_printable_object* deserialize_localized_pokemon(void* serialized_structure){
    //TODO: implementar deserializacion de LOCALIZED

    return NULL;
};