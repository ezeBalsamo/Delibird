#ifndef COMMON_STRUCTURES_H
#define COMMON_STRUCTURES_H

#include <stdint.h>
#include <commons/collections/list.h>

typedef struct Request{
    uint32_t operation;
    void* structure;
}t_request;

typedef struct Serialization_information{
    void* serialized_request;
    uint32_t amount_of_bytes;
}t_serialization_information;

typedef struct Connection_request{
    void* serialized_request;
    int socket_fd;
}t_connection_request;

typedef struct AppearedPokemon{
    char* pokemon_name;
    uint32_t pos_x;
    uint32_t pos_y;
    uint32_t message_id;
}t_appeared_pokemon;

typedef struct NewPokemon{
    char* pokemon_name;
    uint32_t pos_x;
    uint32_t pos_y;
    uint32_t quantity;
    uint32_t message_id;
}t_new_pokemon;

typedef struct CatchPokemon{
    char* pokemon_name;
    uint32_t pos_x;
    uint32_t pos_y;
    uint32_t message_id;
}t_catch_pokemon;

typedef struct CaughtPokemon{
    uint32_t message_id;
    uint32_t caught_status;
}t_caught_pokemon;

typedef struct GetPokemon{
    char* pokemon_name;
}t_get_pokemon;

typedef struct SubscribeMe{
    uint32_t operation_queue;
}t_subscribe_me;

enum Process {
    TEAM, BROKER, GAMECARD, SUSCRIPTOR
};

typedef struct Printable_Object{
    void* object;
    char* (*print_function) (void* object);
}t_printable_object;

enum Operation {
    NEW_POKEMON, APPEARED_POKEMON,
    GET_POKEMON, LOCALIZED_POKEMON,
    CATCH_POKEMON, CAUGHT_POKEMON,
    SUBSCRIBE_ME
};

typedef struct Operation_information{
    uint32_t code;
    char* name;
    int max_arguments_amount;
    t_serialization_information* (*serialize_function) (void* structure);
    t_printable_object* (*deserialize_function) (void* serialized_structure);
}t_operation_information;

typedef struct Process_information{
    uint32_t code;
    char* name;
    t_list* operations;
}t_process_information;

void free_request(t_request* request);
void free_serialization_information(t_serialization_information* serialization_information);
void free_process_information(t_process_information* process_information);
void free_operation_information(t_operation_information* operation_information);
void free_char_array(char**);

char* appeared_pokemon_as_string(t_appeared_pokemon* appeared_pokemon);

char* new_pokemon_as_string(t_new_pokemon* new_pokemon);

char* catch_pokemon_as_string(t_catch_pokemon* catch_pokemon);

char* caught_pokemon_as_string(t_caught_pokemon* caught_pokemon);

char* get_pokemon_as_string(t_get_pokemon* get_pokemon);

char* subscribe_me_as_string(t_subscribe_me* subscribe_me);

void free_printable_object(t_printable_object* printable_object);

#endif //COMMON_STRUCTURES_H
