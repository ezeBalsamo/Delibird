#include "../include/publisher_mode_strategy.h"
#include "../include/entry_point_validator.h"
#include <commons/string.h>
#include <stdlib.h>

t_role_mode* role_mode_strategy;

bool publisher_can_handle(char* process_name){
    return
        string_equals_ignore_case(process_name, "TEAM") ||
        string_equals_ignore_case(process_name, "BROKER") ||
        string_equals_ignore_case(process_name, "GAMECARD");
}

void initialize_publisher_mode_strategy(){
    role_mode_strategy = malloc(sizeof(t_role_mode));
    role_mode_strategy -> can_handle_function = publisher_can_handle;
    role_mode_strategy -> is_valid_operation_function = is_valid_publisher_operation;
}

t_role_mode* publisher_mode_strategy(){
    return role_mode_strategy;
}