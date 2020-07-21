#ifndef DELIBIRD_BUDDY_SYSTEM_MESSAGE_ALLOCATOR_H
#define DELIBIRD_BUDDY_SYSTEM_MESSAGE_ALLOCATOR_H

#include "broker_memory_manager.h"

t_message_allocator* initialize_buddy_system_message_allocator();
uint32_t decimal_position_in_memory(void* pointer);

#endif //DELIBIRD_BUDDY_SYSTEM_MESSAGE_ALLOCATOR_H
