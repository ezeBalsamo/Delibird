#include <commons/collections/list.h>
#include <stdlib.h>
#include "../include/broker_memory_manager.h"
#include "../include/fifo_partition_free_algorithm.h"
#include "../include/broker_memory_algorithms.h"

void* fifo_partition_free_algorithm(t_list* blocks_information){
    //busco el primero no ocupado junto con el indice para consolidarlo
    int nearest_occupied_block_found_index = 0;
    t_block_information* block_found = NULL;

    for(int i = 0; i<list_size(blocks_information);i++){

        block_found = (t_block_information*) list_get(blocks_information,i);
        if (!block_found->is_free){
            nearest_occupied_block_found_index = i; //por este cornudo no hago un list_find
            break;
        }
    }
    void* freed_block_position = block_found->initial_position;
    if (block_found != NULL){
        empty_block_information(block_found);
        //consolido el bloque

        consolidate_block(blocks_information,nearest_occupied_block_found_index);
    }
    return freed_block_position;
}