#ifndef DELIBIRD_BROKER_MEMORY_ALGORITHMS_H
#define DELIBIRD_BROKER_MEMORY_ALGORITHMS_H

t_message_allocator* initialize_message_allocator();

void* get_available_partition_search_algorithm();
void* get_free_partition_algorithm();
void compact_memory_algorithm();


#endif //DELIBIRD_BROKER_MEMORY_ALGORITHMSbroker_H
