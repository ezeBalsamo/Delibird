#include "../include/publisher_mode_strategy.h"
#include "../include/entry_point_validator.h"

t_role_mode* publisher_role_mode_strategy;

bool publisher_can_handle(uint32_t process_code){
    return
        process_code == TEAM ||
        process_code == BROKER ||
        process_code == GAMECARD;
}

char* publisher_process_name_for_connection(){
    t_process_information* process_information = valid_chosen_process();
    return process_information -> name;
}

void initialize_publisher_mode_strategy(){
    publisher_role_mode_strategy = safe_malloc(sizeof(t_role_mode));
    publisher_role_mode_strategy -> can_handle_function = publisher_can_handle;
    publisher_role_mode_strategy -> is_valid_operation_function = is_valid_publisher_operation;
    publisher_role_mode_strategy -> pokemon_operation_structure_function = publisher_pokemon_operation_structure;
    publisher_role_mode_strategy -> process_name_for_connection = publisher_process_name_for_connection();
    publisher_role_mode_strategy -> is_subscriber_mode = false;
}

t_role_mode* publisher_mode_strategy(){
    return publisher_role_mode_strategy;
}