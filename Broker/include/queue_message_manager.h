#ifndef TP_2020_1C_EL_KUELGUE_QUEUE_MESSAGE_MANAGER_H
#define TP_2020_1C_EL_KUELGUE_QUEUE_MESSAGE_MANAGER_H

#include <stdint.h>
#include "../../Utils/include/common_structures.h"

typedef struct Subscriber{
    uint32_t queue;
    int socket_fd;
}t_subscriber;

void initialize_queue_message_manager();

void push_to_queue(uint32_t operation, t_connection_request* connection_request);

void publish(uint32_t queue, t_serialization_information* serialization_information);

void free_all_queues();

void free_subscribers_list();

void free_queue_message_manager();

#endif //TP_2020_1C_EL_KUELGUE_QUEUE_MESSAGE_MANAGER_H
