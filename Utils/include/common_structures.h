#ifndef COMMON_STRUCTURES_H
#define COMMON_STRUCTURES_H

#include <stdint.h>
#include <commons/collections/list.h>

typedef struct Request{
    uint32_t operation;
    void* structure;
}t_request;

enum Process {
    TEAM, BROKER, GAMECARD, SUSCRIPTOR
};

enum Operation {
    APPEARED_POKEMON, CATCH_POKEMON, CAUGHT_POKEMON,
    GET_POKEMON, NEW_POKEMON
};

typedef struct Operation_information{
    int code;
    char* name;
    int max_arguments_amount;
    void (*serialize_function) (char** arguments, void** serialized_structure, uint32_t* amount_of_bytes);
    t_request* (*deserialize_function) (void* serialized_structure);
}t_operation_information;

typedef struct Process_information{
    int code;
    char* name;
    t_list* operations;
}t_process_information;

void free_request(t_request* request);
void free_process_information(t_process_information* process_information);
void free_operation_information(t_operation_information* operation_information);
void free_char_array(char**);

typedef struct NewPokemon{
    char* pokemon;
    uint32_t posx;
    uint32_t posy;
    uint32_t cantidad;
    uint32_t id_mensaje;
}newPokemonArguments;

typedef struct catchPokemon{
    char* pokemon;
    uint32_t posx;
    uint32_t posy;
    uint32_t id_mensaje;
}catchPokemonArguments;




#endif //COMMON_STRUCTURES_H
