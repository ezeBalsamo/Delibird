#ifndef DELIBIRD_BROKER_MESSAGE_ALLOCATOR_H
#define DELIBIRD_BROKER_MESSAGE_ALLOCATOR_H

#include "broker_memory_manager.h"

void initialize_allocation_algorithms();
t_message_allocator* initialize_message_allocator();

int block_index_position(t_block_information* block_to_find,t_list* blocks_information); //Todo: hacer una funcion en list extensions generica.
t_memory_block* build_memory_block_from_message(t_identified_message* message);
t_request* message_request_from_identified_message(t_identified_message* message);
uint32_t block_size_for(t_memory_block *  memory_block_to_save);

#endif //DELIBIRD_BROKER_MESSAGE_ALLOCATOR_H
