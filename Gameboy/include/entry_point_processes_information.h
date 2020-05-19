#ifndef DELIBIRD_ENTRY_POINT_PROCESSES_INFORMATION_H
#define DELIBIRD_ENTRY_POINT_PROCESSES_INFORMATION_H

#include <commons/collections/list.h>
#include "../../Utils/include/common_structures.h"

t_list* broker_operation_information();

t_process_information* broker_process_information();

void initialize_processes_information();

t_process_information* process_information_named(char* process_name);

uint32_t process_information_code_of(char* process_name);

void free_processes_information();

#endif //DELIBIRD_ENTRY_POINT_PROCESSES_INFORMATION_H
