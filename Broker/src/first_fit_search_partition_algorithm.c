#include "../include/broker_memory_manager.h"
#include "../include/first_fit_search_partition_algorithm.h"


t_block_manager* first_fit_search_partition_algorithm(uint32_t message_size,t_list* blocks_manager){

    bool _message_size_fits_partition(void* block_manager){
        bool enough_size_for_message = ((t_block_manager*) block_manager)->block_size > message_size;
        bool block_is_free = ((t_block_manager*) block_manager)->free_block;
    return enough_size_for_message && block_is_free;
    }
    t_block_manager* block_manager_found = (t_block_manager*) list_find(blocks_manager,_message_size_fits_partition);
    return block_manager_found;
}

void* get_first_fit_search_partition_algorithm(){
    return first_fit_search_partition_algorithm;
}