#include <commons/collections/list.h>
#include <broker_memory_manager.h>
#include <broker_memory_algorithms.h>
#include "lru_partition_free_algorithm.h"

void lru_partition_free_algorithm(t_list* blocks_information){
    //busco el primero no ocupado junto con el indice para consolidarlo
    int nearest_occupied_block_found_index = 0;
    t_block_information* block_found = NULL;
    t_list* blocks_used = list_create();

    for(int i = 0; i<list_size(blocks_information);i++){

        block_found = (t_block_information*) list_get(blocks_information,i);
        if (!block_found->is_free){
            nearest_occupied_block_found_index = i; //por este cornudo no hago un list_find
            list_add(blocks_used, block_found);
            break;
        }
    }

    t_block_information* _block_least_recently_used(t_block_information* block_information, t_block_information* another_block_information){
        if(block_information -> memory_block -> lru_value < another_block_information -> memory_block -> lru_value){
            return block_information;
        }
        return another_block_information;
    }

    t_block_information* block_to_free = list_fold(blocks_used, 0,(void* (*) (void*, void*)) _block_least_recently_used);
    empty_block_information(block_to_free);
    //consolido el bloque

    consolidate_block(blocks_information,nearest_occupied_block_found_index);
    list_destroy(blocks_used);
}