#ifndef DELIBIRD_SUBSCRIBER_H
#define DELIBIRD_SUBSCRIBER_H

#include "queue_message_status.h"
#include "queue_context_provider.h"
#include "subscriber_context_provider.h"

void subscribe_client_to_queue(t_subscriber_context* subscriber_context);
bool are_equals_subscribers(t_subscriber_context* subscriber_context, t_subscriber_context* another_subscriber_context);
void send_all_messages(t_subscriber_context* subscriber_context);

#endif //DELIBIRD_SUBSCRIBER_H
