#ifndef DELIBIRD_OPERATIONS_INFORMATION_H
#define DELIBIRD_OPERATIONS_INFORMATION_H

#include "common_structures.h"

t_operation_information* new_pokemon_information();

t_operation_information* appeared_pokemon_information();

t_operation_information* catch_pokemon_information();

t_operation_information* caught_pokemon_information();

t_operation_information* get_pokemon_information();

t_operation_information* localized_pokemon_information();

t_operation_information* subscribe_me_information();

uint32_t queue_code_of(char* queue_name);

bool exists_queue_named(void* queue_name);

t_operation_information* operation_information_with_code(uint32_t operation_code);

#endif //DELIBIRD_OPERATIONS_INFORMATION_H