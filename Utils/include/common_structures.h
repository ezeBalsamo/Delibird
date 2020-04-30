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

enum Process {
    TEAM, BROKER, GAMECARD, SUSCRIPTOR
};

enum Operation {
    APPEARED_POKEMON, CATCH_POKEMON, CAUGHT_POKEMON,
    GET_POKEMON, NEW_POKEMON
};

typedef struct Operation_information{
    uint32_t code;
    char* name;
    int max_arguments_amount;
    t_serialization_information* (*serialize_function) (char** arguments);
    t_request* (*deserialize_function) (void* serialized_structure);
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

#endif //COMMON_STRUCTURES_H
