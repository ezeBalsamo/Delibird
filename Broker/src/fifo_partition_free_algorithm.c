#include <commons/collections/list.h>
#include <stdlib.h>
#include "../include/broker_memory_manager.h"
#include "../include/fifo_partition_free_algorithm.h"

bool is_not_free(void* block_information){
    return !((t_block_information*) block_information) -> is_free;
}

void fifo_partition_free_algorithm(t_list* blocks_information){
    //busco el primero no ocupado

    t_block_information* block_found = (t_block_information*) list_find(blocks_information,is_not_free);

    //vacio este bloque:
    block_found->is_free = true;
    free(block_found->memory_block); // TODO: CHECK
    block_found->memory_block = NULL;
}

void* get_fifo_partition_free_algorithm(){
    return fifo_partition_free_algorithm;
}