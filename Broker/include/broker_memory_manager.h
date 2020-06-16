#ifndef DELIBIRD_BROKER_MEMORY_MANAGER_H
#define DELIBIRD_BROKER_MEMORY_MANAGER_H

#include <stdbool.h>
#include <stdint.h>
#include <commons/collections/list.h>
#include "../../Utils/include/common_structures.h"

typedef struct Memory_block{
    //info del bloque en si:
    uint32_t identified_block_number;
    uint32_t lru_value;
    uint32_t message_size;
    uint32_t message_queue_code;
    uint32_t message_id;
    void* message; //polimorfico segun anexo 2

}t_memory_block;

typedef struct Block_manager{
    bool free_block;
    void* initial_position;
    uint32_t block_size;
    t_memory_block* memory_block;

}t_block_manager;

typedef struct Message_allocator{
    void (*allocate_message_function) (t_identified_message* message, t_list* blocks_manager);

    t_block_manager* (*find_available_partition_algorithm) (uint32_t message_size,t_list* blocks_manager); //first fit, best fit
    void (*free_partition_algorithm) (t_list* blocks_manager); //FIFO/LRU
    void (*compact_memory_algorithm) (t_list* blocks_manager);
    uint32_t min_partition_size;
    uint32_t max_search_tries;

}t_message_allocator;

typedef struct Memory_manager{
    t_list* blocks_manager;

    t_message_allocator* message_allocator;

}t_memory_manager;


void initialize_broker_memory_manager();
void allocate_message(t_identified_message* message);
char* dump_cache();

#endif //DELIBIRD_BROKER_MEMORY_MANAGER_H