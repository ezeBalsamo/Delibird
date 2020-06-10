#ifndef DELIBIRD_SUBSCRIBERS_MANAGER_H
#define DELIBIRD_SUBSCRIBERS_MANAGER_H

#include <commons/collections/list.h>
#include <stdint.h>
#include "publish_message_mode.h"
#include "queue_message_manager.h"

t_queue_context* get_context_of_a_queue(uint32_t queue);
void subscribe_process(int subscriber,uint32_t operation_queue);
void disconnect_subscriber(t_message_status* message_status, int subscriber);
void move_subscriber_to_ACK(t_message_status* message_status, int subscriber);
void send_all_messages(int subscriber, uint32_t operation_queue);
bool equals_subscribers_(const int* subscriber, const int* another_subscriber);

#endif //DELIBIRD_SUBSCRIBERS_MANAGER_H
