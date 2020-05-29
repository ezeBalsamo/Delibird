#ifndef DELIBIRD_OPERATIONS_INFORMATION_H
#define DELIBIRD_OPERATIONS_INFORMATION_H

#include "common_structures.h"

t_operation_information* new_pokemon_operation_information();

t_operation_information* appeared_pokemon_operation_information();

t_operation_information* catch_pokemon_operation_information();

t_operation_information* caught_pokemon_operation_information();

t_operation_information* get_pokemon_operation_information();

t_operation_information* localized_pokemon_operation_information();

t_operation_information* subscribe_me_operation_information();

void initialize_operations_information();

uint32_t queue_code_of(char* queue_name);

bool exists_queue_named(char* queue_name);

char* queue_name_of(uint32_t queue_code);

t_operation_information* operation_information_with_code(uint32_t operation_code);

void free_operations_information();

#endif //DELIBIRD_OPERATIONS_INFORMATION_H
