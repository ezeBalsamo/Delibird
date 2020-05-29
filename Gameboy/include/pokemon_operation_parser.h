#ifndef DELIBIRD_POKEMON_OPERATION_PARSER_H
#define DELIBIRD_POKEMON_OPERATION_PARSER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct Pokemon_operation_parser{
    bool (*can_handle_function) (uint32_t operation_code);
    void* (*parse_function) (char** arguments);
    bool should_build_identified_message;
}t_pokemon_operation_parser;

t_pokemon_operation_parser* pokemon_operation_parser_for(uint32_t operation_code, bool should_build_identified_message);

#endif //DELIBIRD_POKEMON_OPERATION_PARSER_H
