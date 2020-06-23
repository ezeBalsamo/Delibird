#include "../include/broker_memory_manager.h"
#include "../include/first_fit_available_partition_search_algorithm.h"


t_block_information* first_fit_available_partition_search_algorithm(uint32_t message_size,t_list* blocks_information){

    bool _message_size_fits_partition(void* block_information){
        bool enough_size_for_message = ((t_block_information*) block_information)->block_size >= message_size;
        bool block_is_free = ((t_block_information*) block_information)->is_free;
    return enough_size_for_message && block_is_free;
    }
    t_block_information* block_information_found = (t_block_information*) list_find(blocks_information,_message_size_fits_partition);
    return block_information_found;
}
