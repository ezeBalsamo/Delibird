#ifndef COMMON_STRUCTURES_H
#define COMMON_STRUCTURES_H

#include <stdint.h>

typedef struct Request{
    uint32_t operation;
    void* structure;
}request;

typedef enum Process {
    NULL_PROCESS, TEAM, BROKER, GAMECARD, SUSCRIPTOR
} t_process;

typedef enum Operation {
    NULL_OPERATION, APPEARED_POKEMON, CATCH_POKEMON, CAUGHT_POKEMON,
    GET_POKEMON, NEW_POKEMON
} t_operation;

typedef struct AppearedPokemonArguments{
    char* pokemon;
    uint32_t posx;
    uint32_t posy;
    uint32_t id_mensaje;
}appearedPokemonArguments;


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
