#ifndef DELIBIRD_SUBSCRIBER_H
#define DELIBIRD_SUBSCRIBER_H

#include <commons/collections/list.h>
#include <stdint.h>
#include "publisher_message_mode.h"
#include "queue_context_provider.h"

t_queue_context* get_context_of_operation_queue(uint32_t queue);
void subscribe_client_to_queue(int subscriber, uint32_t operation_queue);
void revoke_suscription(t_message_status* message_status,t_queue_context* queue_context, int subscriber);
void send_all_messages(int subscriber, uint32_t operation_queue);

#endif //DELIBIRD_SUBSCRIBER_H
