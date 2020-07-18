#include "../include/broker_memory_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <best_fit_available_partition_search_algorithm.h>
#include <stdlib.h>
#include <lru_partition_free_algorithm.h>
#include <queue_message_status.h>
#include <broker_message_allocator.h>
#include "../include/first_fit_available_partition_search_algorithm.h"
#include "../include/fifo_partition_free_algorithm.h"
#include "../include/broker_memory_algorithms.h"

t_dictionary* algorithms;

void initialize_broker_memory_algorithms(){
    initialize_allocation_algorithms();

    algorithms = dictionary_create();
    dictionary_put(algorithms,"FIFO", (void*) fifo_partition_free_algorithm);
    dictionary_put(algorithms,"LRU", (void*) lru_partition_free_algorithm);
    dictionary_put(algorithms,"FF", (void*) first_fit_available_partition_search_algorithm);
    dictionary_put(algorithms, "BF", (void*) best_fit_available_partition_search_algorithm);
}

void* get_available_partition_search_algorithm() {
    char *search_algorithm = config_get_string_at("ALGORITMO_PARTICION_LIBRE");

    return dictionary_get(algorithms,search_algorithm);
}

void* get_partition_free_algorithm() {
    char *partition_free_algorithm = config_get_string_at("ALGORITMO_REEMPLAZO");

    return dictionary_get(algorithms,partition_free_algorithm);
}

bool can_save_message(t_block_information* block_information, uint32_t message_size, uint32_t min_partition_size){
    bool enough_size_for_message = ((t_block_information*) block_information)->block_size >= message_size;
    bool block_is_free = ((t_block_information*) block_information)->is_free;
    bool block_is_usable = ((t_block_information*) block_information)->block_size >= min_partition_size;
    return enough_size_for_message && block_is_free && block_is_usable;
}

void empty_block_information(t_block_information* block_found){
    block_found->is_free = true;
    //free(block_found->memory_block);
    block_found->memory_block = NULL;
}

bool is_not_free_block(t_block_information* block){
    return !block->is_free;
}

bool is_free_block_in_index(t_list* blocks_information, int index){
    return ((t_block_information*) list_get(blocks_information,index))->is_free;
}

void free_broker_memory_algorithms(){
    dictionary_destroy(algorithms);
}