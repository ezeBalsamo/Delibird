#ifndef ENTRY_POINT_VALIDATOR_H
#define ENTRY_POINT_VALIDATOR_H

#include "entry_point_processes_information.h"

void initialize_entry_point_validator(int arguments_amount, char** arguments);

t_operation_information* valid_chosen_operation();

t_process_information* valid_chosen_process();

bool is_valid_publisher_operation(t_operation_information* operation_information);

bool is_valid_subscriber_operation(t_operation_information* operation_information);

void* publisher_pokemon_operation_structure();

void* subscriber_pokemon_operation_structure();

char* valid_process_name_for_connection();

uint32_t pokemon_operation_code();

void* pokemon_operation_structure();

void* pokemon_operation_sanitizer_function();

bool is_subscriber_mode();

int suscriber_mode_time_in_seconds();

void free_entry_point_validator();

#endif //ENTRY_POINT_VALIDATOR_H

