#include <commons/collections/list.h>
#include <stdlib.h>
#include "../include/broker_memory_manager.h"
#include "../include/fifo_partition_free_algorithm.h"
#include "../../Utils/include/t_list_extension.h"


void empty_block_information(t_block_information* block_found){
    block_found->is_free = true;
    free(block_found->memory_block);
    block_found->memory_block = NULL;
}

void consolidate_block_with(t_block_information* master_block,t_block_information* block_to_be_consolidated){
    master_block->block_size += block_to_be_consolidated ->block_size;

    if(master_block->initial_position > block_to_be_consolidated ->initial_position){

        master_block->initial_position = block_to_be_consolidated ->initial_position;
    }

    free(block_to_be_consolidated);
}

bool is_free_block_in_index(t_list* blocks_information, int index){
    return ((t_block_information*) list_get(blocks_information,index+1))->is_free;
}

void consolidate_block(t_list* blocks_information,int index_of_block_to_consolidate){
    t_block_information* master_block = (t_block_information*) list_get(blocks_information,index_of_block_to_consolidate);

    if (is_valid_index(blocks_information,index_of_block_to_consolidate+1)){
        if(is_free_block_in_index(blocks_information,index_of_block_to_consolidate+1)){ //en caso de que no sea lazy evaluation

            t_block_information* block_to_be_consolidated = (t_block_information*) list_remove(blocks_information,index_of_block_to_consolidate+1);

            consolidate_block_with(master_block,block_to_be_consolidated);
        }
    }

    if (is_valid_index(blocks_information,index_of_block_to_consolidate-1)){
        if(is_free_block_in_index(blocks_information, index_of_block_to_consolidate-1)){
            t_block_information* block_to_be_consolidated = (t_block_information*) list_remove(blocks_information,index_of_block_to_consolidate-1);

            consolidate_block_with(master_block,block_to_be_consolidated);
        }
    }

}

void fifo_partition_free_algorithm(t_list* blocks_information){
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

    empty_block_information(block_found);
    //consolido el bloque

    consolidate_block(blocks_information,nearest_occupied_block_found_index);

}

void* get_fifo_partition_free_algorithm(){
    return fifo_partition_free_algorithm;
}