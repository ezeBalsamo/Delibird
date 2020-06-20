#ifndef DELIBIRD_BROKER_MEMORY_MANAGER_H
#define DELIBIRD_BROKER_MEMORY_MANAGER_H

#include <stdbool.h>
#include <stdint.h>
#include <commons/collections/list.h>
#include "../../Utils/include/common_structures.h"

typedef struct Memory_block{
    //info del bloque en si: (metadata)
    uint32_t message_id;
    uint32_t message_operation;
    uint32_t message_size;
    uint32_t lru_value;

    void* message; //polimorfico segun anexo 2

}t_memory_block;

typedef struct Block_information{
    bool is_free;
    void* initial_position;
    uint32_t block_size;
    t_memory_block* memory_block;

}t_block_information;

typedef struct Message_allocator{
    void (*allocate_message_function) (t_identified_message* message, t_list* blocks_information);

    t_block_information* (*find_available_partition_algorithm) (uint32_t message_size,t_list* blocks_information); //first fit, best fit
    void (*free_partition_algorithm) (t_list* blocks_information); //FIFO/LRU
    void (*compact_memory_algorithm) (t_list* blocks_information);
    uint32_t min_partition_size;
    uint32_t max_search_tries;

}t_message_allocator;

typedef struct Memory_manager{

    t_list* blocks_information;

    t_message_allocator* message_allocator;

}t_memory_manager;


void initialize_broker_memory_manager();
void free_broker_memory_manager();
void allocate_message(t_identified_message* message);
char* dump_cache();

#endif //DELIBIRD_BROKER_MEMORY_MANAGER_H