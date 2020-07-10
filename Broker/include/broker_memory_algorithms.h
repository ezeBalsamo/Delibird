#ifndef DELIBIRD_BROKER_MEMORY_ALGORITHMS_H
#define DELIBIRD_BROKER_MEMORY_ALGORITHMS_H

void initialize_broker_memory_algorithms();

void* get_available_partition_search_algorithm();
void* get_partition_free_algorithm();
void memory_compaction_algorithm();
bool can_save_message(t_block_information* block_information, uint32_t message_size, uint32_t min_partition_size);
void empty_block_information(t_block_information* block_found);
void consolidate_block(t_list* blocks_information,int index_of_block_to_consolidate);
bool is_not_free_block(t_block_information* block);

#endif //DELIBIRD_BROKER_MEMORY_ALGORITHMS_H