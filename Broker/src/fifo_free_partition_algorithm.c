#include <commons/collections/list.h>
#include <stdlib.h>
#include "../include/broker_memory_manager.h"
#include "../include/fifo_free_partition_algorithm.h"


void fifo_partition_algorithm(t_list* blocks_manager){
    bool _is_not_free(void* block_manager){
        bool is_not_free = ((t_block_manager*) block_manager) -> free_block == false;
        return is_not_free;
    }
    t_block_manager* block_found = (t_block_manager*) list_find(blocks_manager,_is_not_free);

    //vacio este bloque:
    block_found->free_block = true;
    free(block_found->memory_block); // TODO: CHECK
    block_found->memory_block = NULL;
}

void* get_fifo_free_partition_algorithm(){
    return fifo_partition_algorithm;
}