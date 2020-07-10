#include "../include/broker_memory_manager.h"
#include "../include/first_fit_available_partition_search_algorithm.h"
#include "../include/broker_memory_algorithms.h"

t_block_information* first_fit_available_partition_search_algorithm(uint32_t message_size,t_list* blocks_information, uint32_t min_partition_size){

    bool _message_size_fits_partition(t_block_information* block_information){
        return can_save_message(block_information, message_size, min_partition_size);
    }
    t_block_information* block_information_found = (t_block_information*) list_find(blocks_information, (bool (*)(void *)) _message_size_fits_partition);
    return block_information_found;
}