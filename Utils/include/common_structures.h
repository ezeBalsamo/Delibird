#ifndef COMMON_STRUCTURES_H
#define COMMON_STRUCTURES_H

#include <stdint.h>
#include <commons/collections/list.h>

typedef struct Request{
    uint32_t operation;
    void* structure;
    void (*sanitizer_function) (struct Request* request);
}t_request;

typedef struct Serialization_information{
    void* serialized_request;
    uint32_t amount_of_bytes;
}t_serialization_information;

typedef struct Connection_request{
    void* serialized_request;
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

typedef struct Subscribe_me{
    uint32_t operation_queue;
}t_subscribe_me;

typedef struct Identified_message{
    uint32_t message_id;
    t_request* request;
}t_identified_message;

enum Process {
    TEAM, BROKER, GAMECARD, SUSCRIPTOR
};

typedef struct Printable_Object{
    uint32_t code;
    char* (*print_function) (void* object);
}t_printable_object;

enum Operation {
    NEW_POKEMON, APPEARED_POKEMON,
    GET_POKEMON, LOCALIZED_POKEMON,
    CATCH_POKEMON, CAUGHT_POKEMON,
    SUBSCRIBE_ME, IDENTIFIED_MESSAGE
};

typedef struct Operation_information{
    uint32_t code;
    char* name;
    int max_arguments_amount;
    t_serialization_information* (*serialize_function) (void* structure);
    t_request* (*deserialize_function) (void* serialized_structure);
}t_operation_information;

typedef struct Process_information{
    uint32_t code;
    char* name;
    t_list* operations;
}t_process_information;

void free_request(t_request* request);
void free_identified_request(t_request* request);
void free_connection_request(t_connection_request* connection_request);
void free_serialization_information(t_serialization_information* serialization_information);
void free_process_information(t_process_information* process_information);
void free_char_array(char**);

void free_printable_object(t_printable_object* printable_object);

#endif //COMMON_STRUCTURES_H
