#include "../include/broker_memory_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include <commons/string.h>
#include <broker_memory_algorithms.h>
#include <broker_logs_manager.h>
#include <cache_information_builder.h>
#include <stdlib.h>

t_memory_manager* memory_manager;

void initialize_broker_memory_manager(){

    memory_manager = safe_malloc(sizeof(t_memory_manager));
    memory_manager->message_allocator = initialize_message_allocator();

    memory_manager->blocks_information = list_create();

    uint32_t memory_size = config_get_int_at("TAMANO_MEMORIA");
    //initial block manager, with all the memory
    t_block_information* initial_block_information = safe_malloc(sizeof(t_block_information));
    initial_block_information->is_free = true;
    initial_block_information->block_size = memory_size;
    initial_block_information->initial_position = safe_malloc(memory_size);
    initial_block_information->memory_block = NULL;

    list_add(memory_manager->blocks_information,(void*) initial_block_information);

    log_successful_initialize_broker_memory_manager();
}

void allocate_message(t_identified_message* message){
    memory_manager->message_allocator->allocate_message_function (message,memory_manager->blocks_information);
}

char* dump_cache(){
    return cache_information_builder(memory_manager->blocks_information);
}
void free_broker_memory_manager(){
    list_destroy_and_destroy_elements(memory_manager->blocks_information,(void (*)(void *)) free);
    free(memory_manager->message_allocator);
    free(memory_manager);
}