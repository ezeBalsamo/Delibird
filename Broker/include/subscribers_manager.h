#ifndef DELIBIRD_SUBSCRIBERS_MANAGER_H
#define DELIBIRD_SUBSCRIBERS_MANAGER_H

#include <commons/collections/list.h>
#include <stdint.h>
#include "publish_message_mode.h"

void initialize_subscribers_manager();

t_list* get_subscribers_of_a_queue(uint32_t queue);
void subscribe_process(int subscriber,uint32_t operation_queue);
void move_subscriber_to_ACK(t_message_status* message_status, int subscriber);
void send_all_messages(int subscriber, uint32_t operation_queue);
void free_subscribers_manager();

#endif //DELIBIRD_SUBSCRIBERS_MANAGER_H
