#include "../include/broker_memory_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/pthread_wrapper.h"
#include <commons/string.h>
#include <broker_memory_algorithms.h>
#include <broker_logs_manager.h>
#include <cache_information_builder.h>
#include <stdlib.h>
#include <broker_message_allocator.h>
#include <pthread.h>

t_list* blocks_information;
pthread_mutex_t memory_mutex;

t_block_information* initialize_first_block_information(){
    uint32_t memory_size = config_get_int_at("TAMANO_MEMORIA");

    t_block_information* initial_block_information = safe_malloc(sizeof(t_block_information));
    initial_block_information->is_free = true;
    initial_block_information->block_size = memory_size;
    initial_block_information->initial_position = safe_malloc(memory_size);
    initial_block_information->memory_block = NULL;

    consider_as_garbage(initial_block_information -> initial_position, free);
    return initial_block_information;
}

void initialize_broker_memory_manager(){

    initialize_broker_memory_algorithms();
    safe_mutex_initialize(&memory_mutex);
    initialize_message_allocator();

    t_block_information* initial_block_information = initialize_first_block_information();

    blocks_information = list_create();
    list_add(blocks_information,(void*) initial_block_information);

    log_successful_initialize_broker_memory_manager();
}

void allocate_message_using(uint32_t message_id, t_deserialization_information* deserialization_information){
    pthread_mutex_lock(&memory_mutex);
    allocate_with_message_allocator_in_blocks_information(message_id, deserialization_information, blocks_information);
    pthread_mutex_unlock(&memory_mutex);
}

char* dump_cache(){
    return cache_information_builder(blocks_information);
}

t_block_information* find_block_information_with_id(uint32_t message_id){
    bool _block_information_with_id(t_block_information* block_information){
        return block_information -> memory_block -> message_id == message_id;
    }

    t_list* blocks_information_occupied = list_filter(blocks_information, (bool (*) (void*)) is_not_free_block);
    t_block_information* block_information_found = list_find(blocks_information_occupied, (bool (*) (void*)) _block_information_with_id);

    if(block_information_found == NULL){
//        log_block_information_with_id_not_found();
    }

    list_destroy(blocks_information_occupied);

    return block_information_found;
}

void update_lru_for(uint32_t message_id){

    t_block_information* block_information_found = find_block_information_with_id(message_id);
    block_information_found -> memory_block -> lru_value = current_time_in_milliseconds();
}

t_memory_block* get_memory_block_from_memory(uint32_t message_id){

    t_block_information* block_information_found = find_block_information_with_id(message_id);
    return block_information_found -> memory_block;
}

void free_block_information(t_block_information* block_information){
    if (!block_information->is_free){
        free(block_information -> memory_block);
    }
    free(block_information);
}

void free_broker_memory_manager(){
    pthread_mutex_destroy(&memory_mutex);
    list_destroy_and_destroy_elements(blocks_information, (void (*)(void *)) free_block_information);
    free_message_allocator();
}