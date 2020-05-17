#ifndef DELIBIRD_ROLE_MODE_STRATEGY_H
#define DELIBIRD_ROLE_MODE_STRATEGY_H

#include <stdbool.h>
#include "../../Utils/include/common_structures.h"

typedef struct Role_mode_strategy{
    bool (*can_handle_function) (char* process_name);
    bool (*is_valid_operation_function) (t_operation_information*);
}t_role_mode;

t_role_mode* role_mode_for(char* process_name);

void free_role_mode_strategy();

#endif //DELIBIRD_ROLE_MODE_STRATEGY_H
