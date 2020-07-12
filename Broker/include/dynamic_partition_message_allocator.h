#ifndef DELIBIRD_DYNAMIC_PARTITION_MESSAGE_ALLOCATOR_H
#define DELIBIRD_DYNAMIC_PARTITION_MESSAGE_ALLOCATOR_H

#include "broker_memory_manager.h"

t_message_allocator* initialize_dynamic_partition_message_allocator();

int block_index_position(t_block_information* block_to_find,t_list* blocks_information); //Todo: hacer una funcion en list extensions generica.

#endif //DELIBIRD_DYNAMIC_PARTITION_MESSAGE_ALLOCATOR_H
