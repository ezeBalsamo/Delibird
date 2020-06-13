#ifndef DELIBIRD_BROKER_MEMORY_MANAGER_H
#define DELIBIRD_BROKER_MEMORY_MANAGER_H

#include <stdbool.h>
#include <stdint.h>
#include <commons/collections/list.h>
#include "../../Utils/include/common_structures.h"

typedef struct Memory_block{
    //info del bloque en si:
    uint32_t identified_block_number;
    uint32_t lru_value;
    uint32_t message_size;
    uint32_t message_id;
    void* message; //polimorfico segun anexo 2

}t_memory_block;

typedef struct Block_manager{
    bool free_block;
    void* initial_position;
    uint32_t block_size;
    t_memory_block* memory_block;

}t_block_manager;

typedef struct Memory_manager{
    t_list* blocks_manager;
    void (*allocate_message_function) (t_identified_message* message);

    t_block_manager* (*find_available_partition_algorithm) (uint32_t message_size); //first fit, best fit
    void (*free_partition_algorithm) (); //
    void (*compact_memory_function) ();
    //algoritmo para elegir particion a eliminar
    uint32_t min_partition_size;
    uint32_t max_search_tries;

}t_memory_manager;


void initialize_broker_memory_manager();
void allocate_message(t_identified_message* message);

#endif //DELIBIRD_BROKER_MEMORY_MANAGER_H