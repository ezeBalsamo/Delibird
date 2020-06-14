#ifndef DELIBIRD_QUEUE_MESSAGE_STATUS_H
#define DELIBIRD_QUEUE_MESSAGE_STATUS_H

#include "../../Utils/include/common_structures.h"
#include "queue_context_provider.h"
#include "subscriber_context_provider.h"

typedef struct Message_status {
    t_identified_message* identified_message;
    t_list* subscribers_to_send;
    t_list* subscribers_who_received;
} t_message_status;

t_message_status* create_message_status_for(t_identified_message* identified_message);
void move_subscriber_to_ACK(t_message_status* message_status, t_subscriber_context* subscriber_context);
t_request* create_request_from(t_message_status* message_status);
void* receive_ack_thread(void* subscriber_fd);
uint32_t join_reception_for_ack_thread(pthread_t waiting_for_ack_thread, t_subscriber_context* subscriber_context, t_message_status* message_status);

void free_message_status(t_message_status* message_status);

#endif //DELIBIRD_QUEUE_MESSAGE_STATUS_H
