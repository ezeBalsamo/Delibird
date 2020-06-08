#ifndef COMMON_STRUCTURES_H
#define COMMON_STRUCTURES_H

#include <stdint.h>
#include <commons/collections/list.h>
#include <stddef.h>
typedef struct Request{
    uint32_t operation;
    void* structure;
    void (*sanitizer_function) (void* request_structure);
}t_request;

typedef struct Serialization_information{
    void* serialized_request;
    uint32_t amount_of_bytes;
}t_serialization_information;

typedef struct Connection_request{
    t_request* request;
    int socket_fd;
}t_connection_request;

typedef struct Appeared_pokemon{
    char* pokemon_name;
    uint32_t pos_x;
    uint32_t pos_y;
}t_appeared_pokemon;

typedef struct New_pokemon{
    char* pokemon_name;
    uint32_t pos_x;
    uint32_t pos_y;
    uint32_t quantity;
}t_new_pokemon;

typedef struct Catch_pokemon{
    char* pokemon_name;
    uint32_t pos_x;
    uint32_t pos_y;
}t_catch_pokemon;

typedef struct Caught_pokemon{
    uint32_t caught_status;
}t_caught_pokemon;

typedef struct Get_pokemon{
    char* pokemon_name;
}t_get_pokemon;

typedef struct Localized_pokemon{
    char* pokemon_name;
    uint32_t quantity;
    t_list* positions;
}t_localized_pokemon;

typedef struct Subscribe_me{
    uint32_t operation_queue;
}t_subscribe_me;

typedef struct Identified_message{
    uint32_t message_id;
    t_request* request;
}t_identified_message;

typedef struct Serializable_Object{
    uint32_t code;
    t_serialization_information* (*serialize_function) (void* structure);
    t_request* (*deserialize_function) (void* serialized_structure);
}t_serializable_object;

enum Operation {
    NEW_POKEMON, APPEARED_POKEMON,
    GET_POKEMON, LOCALIZED_POKEMON,
    CATCH_POKEMON, CAUGHT_POKEMON,
    SUBSCRIBE_ME, IDENTIFIED_MESSAGE
};

uint32_t internal_operation_in(t_identified_message* identified_message);
uint32_t internal_operation_in_correlative(t_identified_message* correlative_identified_message);
void* internal_object_in(t_identified_message* identified_message);
void* internal_object_in_correlative(t_identified_message* correlative_identified_message);
void* internal_request_in_correlative(t_identified_message* correlative_identified_message);

void initialize_signal_handler();
void* safe_malloc(size_t size);
t_identified_message* create_identified_message(uint32_t message_id, t_request* request);
t_connection_request* create_connection_request(int connection_fd, t_request* request);
void free_request(t_request* request);
void free_identified_message(t_identified_message* identified_message);
void free_connection_request(t_connection_request* connection_request);
void free_serialization_information(t_serialization_information* serialization_information);
void free_localized_pokemon(t_localized_pokemon* localized_pokemon);
void free_char_array(char**);

#endif //COMMON_STRUCTURES_H