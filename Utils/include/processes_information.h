#ifndef DELIBIRD_PROCESSES_INFORMATION_H
#define DELIBIRD_PROCESSES_INFORMATION_H

#include "common_structures.h"

void initialize_processes_information();

t_process_information* process_information_named(char* name);

t_operation_information* operation_information_with_code(uint32_t operation_code);

void free_processes_information();

#endif //DELIBIRD_PROCESSES_INFORMATION_H
