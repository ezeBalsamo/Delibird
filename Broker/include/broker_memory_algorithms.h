#ifndef DELIBIRD_BROKER_MEMORY_ALGORITHMS_H
#define DELIBIRD_BROKER_MEMORY_ALGORITHMS_H

void initialize_broker_memory_algorithms();

void* get_available_partition_search_algorithm();
void* get_partition_free_algorithm();

bool can_save_message(t_block_information* block_information, uint32_t message_size, uint32_t min_partition_size);
void empty_block_information(t_block_information* block_found);
bool is_not_free_block(t_block_information* block);
bool is_free_block_in_index(t_list* blocks_information, int index);
void free_broker_memory_algorithms();

#endif //DELIBIRD_BROKER_MEMORY_ALGORITHMS_H