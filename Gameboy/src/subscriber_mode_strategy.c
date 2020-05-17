#include "../include/subscriber_mode_strategy.h"
#include "../include/entry_point_validator.h"
#include <stdlib.h>
#include <commons/string.h>

t_role_mode* role_mode_strategy;

bool subscriber_can_handle(char* process_name){
    return string_equals_ignore_case(process_name,"SUSCRIPTOR");
}

void initialize_subscriber_mode_strategy(){
    role_mode_strategy = malloc(sizeof(t_role_mode));
    role_mode_strategy -> can_handle_function = subscriber_can_handle;
    role_mode_strategy -> is_valid_operation_function = is_valid_subscriber_operation;
}
t_role_mode* subscriber_mode_strategy(){
    return role_mode_strategy;
}