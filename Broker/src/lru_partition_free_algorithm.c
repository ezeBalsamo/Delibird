#include <commons/collections/list.h>
#include <broker_memory_manager.h>
#include <broker_memory_algorithms.h>
#include <dynamic_partition_message_allocator.h>
#include "lru_partition_free_algorithm.h"

bool is_not_free_block(t_block_information* block){
    return !block->is_free;
}
void* lru_partition_free_algorithm(t_list* blocks_information){
    t_block_information* block_to_free = NULL;
    t_block_information* _block_least_recently_used(t_block_information* block_information, t_block_information* another_block_information){
        if(block_information -> memory_block -> lru_value < another_block_information -> memory_block -> lru_value){
            return block_information;
        }
        return another_block_information;
    }
    t_list* blocks_occupied = list_filter(blocks_information, (bool (*)(void *)) is_not_free_block);

    if (list_size(blocks_occupied)>0){
        block_to_free = list_fold(blocks_occupied, list_get(blocks_occupied,0),(void* (*) (void*, void*)) _block_least_recently_used);
    }
    void* freed_block_position = block_to_free->initial_position;

     if (block_to_free != NULL){
        empty_block_information(block_to_free);
        //consolido el bloque
        int block_index_to_free = block_index_position(block_to_free,blocks_information);

        consolidate_block(blocks_information,block_index_to_free);
    }
     return freed_block_position;
}