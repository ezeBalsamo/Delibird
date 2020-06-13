#include "../include/broker_memory_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include <commons/string.h>
#include <dynamic_partition_memory_manager.h>

t_memory_manager* memory_manager;

bool is_dynamic_memory_algorithm(char* memory_algorithm){
    return string_equals_ignore_case("PD",memory_algorithm);
}

bool is_buddy_system_memory_algorithm(char* memory_algorithm){
    return string_equals_ignore_case("BD",memory_algorithm);
}


void initialize_broker_memory_manager(){

    char* memory_algorithm = config_get_string_at("ALGORITMO_MEMORIA");

    if(is_dynamic_memory_algorithm(memory_algorithm)){

        memory_manager = initialize_dynamic_partition_memory_manager();

    }else if (is_buddy_system_memory_algorithm(memory_algorithm)){

        //initialize_buddy_system_memory_manager();
    }else{
        //log error
    }
}

void allocate_message(t_identified_message* message){
    memory_manager->allocate_message_function (message);
}