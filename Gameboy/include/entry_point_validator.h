#ifndef ENTRY_POINT_VALIDATOR_H
#define ENTRY_POINT_VALIDATOR_H

#include "../../Utils/include/common_structures.h"

void initialize_entry_point_validator(int arguments_amount, char** arguments);

t_operation_information* valid_chosen_operation();

t_process_information* valid_chosen_process();

char** operation_arguments();

void free_entry_point_validator();

#endif //ENTRY_POINT_VALIDATOR_H

