#include "../include/broker_memory_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include <commons/string.h>
#include <broker_memory_algorithms.h>
#include <broker_logs_manager.h>

t_memory_manager* memory_manager;

void initialize_broker_memory_manager(){

    memory_manager = safe_malloc(sizeof(t_memory_manager));
    memory_manager->message_allocator = initialize_message_allocator();

    memory_manager->blocks_manager = list_create();

    uint32_t memory_size = config_get_int_at("TAMANO_MEMORIA");
    //initial block manager, with all the memory
    t_block_manager* initial_block_manager = safe_malloc(sizeof(t_block_manager));
    initial_block_manager->free_block = true;
    initial_block_manager->block_size = memory_size;
    initial_block_manager->initial_position = safe_malloc(memory_size);
    initial_block_manager->memory_block = NULL;

    list_add(memory_manager->blocks_manager,(void*) initial_block_manager);

    log_successful_initialize_broker_memory_manager();
}

void allocate_message(t_identified_message* message){
    memory_manager->message_allocator->allocate_message_function (message,memory_manager->blocks_manager);
}