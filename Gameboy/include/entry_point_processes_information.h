#ifndef DELIBIRD_ENTRY_POINT_PROCESSES_INFORMATION_H
#define DELIBIRD_ENTRY_POINT_PROCESSES_INFORMATION_H

#include "../../Utils/include/common_structures.h"
#include <stdbool.h>

typedef struct Process_information{
    uint32_t code;
    char* name;
    t_list* operations;
}t_process_information;

typedef struct Operation_information{
    char* name;
    int arguments_amount;
    bool has_identified_message;
    t_serializable_object* serializable_object;
}t_operation_information;

enum Process {
    TEAM, BROKER, GAMECARD, SUSCRIPTOR
};

char* broker_process_name();

void initialize_processes_information();

t_process_information* process_information_named(char* process_name);

uint32_t process_information_code_of(char* process_name);

void free_processes_information();

#endif //DELIBIRD_ENTRY_POINT_PROCESSES_INFORMATION_H
