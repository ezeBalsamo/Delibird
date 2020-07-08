#include <broker_memory_manager.h>
#include "../include/best_fit_available_partition_search_algorithm.h"
#include "../include/broker_memory_algorithms.h"

t_block_information* best_fit_available_partition_search_algorithm(uint32_t message_size,t_list* blocks_information, uint32_t min_partition_size){
    t_block_information* block_information_found = NULL;

    bool is_available_partition_for_free_victim(t_block_information* block_information){
        return can_save_message(block_information, message_size, min_partition_size) && block_information->is_free;
    }

    t_block_information* _best_block_information(t_block_information* block_information, t_block_information* another_block_information){
        if(block_information -> block_size - message_size < another_block_information -> block_size - message_size){
            return block_information;
        }
        return another_block_information;
    }

    t_list* blocks_information_availables = list_filter(blocks_information, (bool (*)(void *)) is_available_partition_for_free_victim);

    if (list_size(blocks_information_availables) > 0){
        block_information_found = list_fold(blocks_information_availables,list_get(blocks_information_availables,0), (void* (*)(void *, void*)) _best_block_information);
    }

    return block_information_found;
}