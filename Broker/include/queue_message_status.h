#ifndef DELIBIRD_QUEUE_MESSAGE_STATUS_H
#define DELIBIRD_QUEUE_MESSAGE_STATUS_H

#include "../../Utils/include/common_structures.h"
#include "queue_context_provider.h"
#include "subscriber_context_provider.h"
#include "broker_memory_manager.h"

typedef struct Message_status {
    uint32_t message_id;
    uint32_t operation_queue;
    t_list* subscribers_to_send;
    t_list* subscribers_who_received;
} t_message_status;

t_message_status* create_message_status_using(uint32_t message_id, t_deserialization_information* deserialization_information);
void move_subscriber_to_ACK(t_message_status* message_status, t_subscriber_context* subscriber_context);
t_request* create_request_from(t_memory_block* memory_block);
void* receive_ack_thread(void* subscriber_fd);
void* join_reception_for_ack_thread(pthread_t waiting_for_ack_thread, t_subscriber_context* subscriber_context, t_message_status* message_status, t_queue_context* queue_context);
void delete_message(uint32_t operation_message, uint32_t message_id, char* reason);

void free_message_status(t_message_status* message_status);

#endif //DELIBIRD_QUEUE_MESSAGE_STATUS_H