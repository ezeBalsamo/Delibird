#include <commons/collections/list.h>
#include <stdlib.h>
#include "../include/broker_memory_manager.h"
#include "../include/fifo_partition_free_algorithm.h"
#include "../include/broker_memory_algorithms.h"

t_block_information* fifo_partition_free_algorithm(t_list* blocks_information){
    return (t_block_information*) list_find(blocks_information, (bool (*)(void *)) is_not_free_block);
}