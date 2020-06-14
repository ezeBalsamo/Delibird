#ifndef DELIBIRD_BROKER_MEMORY_ALGORITHMS_H
#define DELIBIRD_BROKER_MEMORY_ALGORITHMS_H

void* get_allocate_message_algorithm();

void* get_search_partition_algorithm();
void* get_free_partition_algorithm();
void compact_memory_algorithm();


#endif //DELIBIRD_BROKER_MEMORY_ALGORITHMS_H
