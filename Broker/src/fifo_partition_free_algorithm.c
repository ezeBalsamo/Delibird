#include <commons/collections/list.h>
#include "../include/broker_memory_manager.h"
#include "../include/fifo_partition_free_algorithm.h"
#include "../include/broker_memory_algorithms.h"

t_block_information* fifo_partition_free_algorithm(t_list* blocks_information){
    t_block_information* block_to_free = NULL;
    t_block_information* _block_with_oldest_id(t_block_information* block_information, t_block_information* another_block_information){
        if(block_information -> memory_block -> memory_block_id  < another_block_information -> memory_block -> memory_block_id){
            return block_information;
        }
        return another_block_information;
    }
    t_list* blocks_occupied = list_filter(blocks_information, (bool (*)(void *)) is_not_free_block);

    if (list_size(blocks_occupied)>0){
        block_to_free = list_fold(blocks_occupied, list_get(blocks_occupied,0),(void* (*) (void*, void*)) _block_with_oldest_id);
    }

    return block_to_free;
}