#ifndef DELIBIRD_DYNAMIC_PARTITION_MEMORY_MANAGER_H
#define DELIBIRD_DYNAMIC_PARTITION_MEMORY_MANAGER_H

#include "broker_memory_manager.h"

t_message_allocator* initialize_dynamic_partition_message_allocator();

void* get_dynamic_partition_allocate_message_algorithm();

#endif //DELIBIRD_DYNAMIC_PARTITION_MEMORY_MANAGER_H
