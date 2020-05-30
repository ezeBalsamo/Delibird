#ifndef DELIBIRD_ROLE_MODE_STRATEGY_H
#define DELIBIRD_ROLE_MODE_STRATEGY_H

#include <stdbool.h>
#include "entry_point_processes_information.h"

typedef struct Role_mode_strategy{
    bool (*can_handle_function) (uint32_t process_code);
    bool (*is_valid_operation_function) (t_operation_information*);
    void* (*pokemon_operation_structure_function) ();
    char* process_name_for_connection;
    bool is_subscriber_mode;
}t_role_mode;

t_role_mode* role_mode_for(uint32_t process_code);

void free_role_mode_strategy();

#endif //DELIBIRD_ROLE_MODE_STRATEGY_H
