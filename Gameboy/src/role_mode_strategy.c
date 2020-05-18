#include <commons/collections/list.h>
#include "../include/role_mode_strategy.h"
#include "../include/subscriber_mode_strategy.h"
#include "../include/publisher_mode_strategy.h"
#include "../include/entry_point_logs_manager.h"
#include <stdlib.h>

t_list* role_modes_strategy;

void initialize_role_mode_strategy(){
    initialize_subscriber_mode_strategy();
    initialize_publisher_mode_strategy();

    role_modes_strategy = list_create();
    list_add(role_modes_strategy, (void*) subscriber_mode_strategy());
    list_add(role_modes_strategy, (void*) publisher_mode_strategy());
}

t_role_mode* role_mode_for(uint32_t process_code){

    initialize_role_mode_strategy();

    bool _can_handle(void* role_mode_strategy){
        t_role_mode* cast_role_mode_strategy = (t_role_mode*) role_mode_strategy;
        return (*(cast_role_mode_strategy -> can_handle_function)) (process_code);
    }

    return list_find(role_modes_strategy, _can_handle);
}

void free_role_mode_strategy(){
    list_destroy_and_destroy_elements(role_modes_strategy, free);
}