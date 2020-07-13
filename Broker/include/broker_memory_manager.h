#ifndef DELIBIRD_BROKER_MEMORY_MANAGER_H
#define DELIBIRD_BROKER_MEMORY_MANAGER_H

#include <stdbool.h>
#include <stdint.h>
#include <commons/collections/list.h>
#include "../../Utils/include/common_structures.h"

typedef struct Memory_block{
    //info del bloque en si: (metadata)
    uint32_t message_id;
    uint32_t correlative_message_id;
    uint32_t message_operation;
    uint32_t message_size;
    uint32_t lru_value;
    uint64_t memory_block_id; //id to handle FIFO order

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

    t_block_information* (*available_partition_search_algorithm) (uint32_t message_size,t_list* blocks_information, uint32_t min_partition_size); //first fit, best fit
    void* (*partition_free_algorithm) (t_list* blocks_information); //FIFO/LRU
    void (*memory_compaction_algorithm) (t_list* blocks_information);
    uint32_t min_partition_size;
    uint32_t max_search_tries;
    uint32_t max_partition_size;

}t_message_allocator;


void initialize_broker_memory_manager();
void free_broker_memory_manager();
void allocate_message(t_identified_message* message);
char* dump_cache();
void update_lru_for(uint32_t message_id);
t_block_information* find_block_information_with_id(uint32_t message_id);
t_memory_block* get_memory_block_from_memory(uint32_t message_id);
void free_block_information(t_block_information* block_information);

#endif //DELIBIRD_BROKER_MEMORY_MANAGER_H