#ifndef DELIBIRD_BROKER_MESSAGE_ALLOCATOR_H
#define DELIBIRD_BROKER_MESSAGE_ALLOCATOR_H

#include "broker_memory_manager.h"

void initialize_allocation_algorithms();
void initialize_message_allocator();
void free_message_allocator();

//common block operations for each message_allocator
void allocate_with_message_allocator_in_blocks_information(t_identified_message* identified_message, t_list* blocks_information);
int block_index_position(t_block_information* block_to_find,t_list* blocks_information); //Todo: hacer una funcion en list extensions generica.
t_memory_block* build_memory_block_from(t_identified_message* message, t_block_information* block_information);
uint32_t get_size_of(t_identified_message* message);
t_request* message_request_from_identified_message(t_identified_message* message);
uint32_t block_size_for(t_memory_block *  memory_block_to_save);
void consolidate_block_with(t_block_information* master_block,t_block_information* block_to_be_consolidated);
uint64_t get_next_fifo_id();

#endif //DELIBIRD_BROKER_MESSAGE_ALLOCATOR_H
