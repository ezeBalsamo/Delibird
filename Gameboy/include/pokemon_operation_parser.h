#ifndef DELIBIRD_POKEMON_OPERATION_PARSER_H
#define DELIBIRD_POKEMON_OPERATION_PARSER_H

#include <stdbool.h>

typedef struct Pokemon_operation_parser{
    bool (*can_handle_function) (char* operation_code);
    void* (*parse_function) (char** arguments);
}t_pokemon_operation_parser;

t_pokemon_operation_parser* pokemon_operation_parser_for(char* operation_name);

#endif //DELIBIRD_POKEMON_OPERATION_PARSER_H
