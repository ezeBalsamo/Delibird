#ifndef DELIBIRD_PUBLISHER_H
#define DELIBIRD_PUBLISHER_H

#include "queue_message_status.h"

typedef struct Subscriber_ack_thread{
    pthread_t subscriber_thread;
    t_subscriber_context* subscriber_context;
    t_message_status* message_status;
} t_subscriber_ack_thread;

void push_to_queue(t_message_status* message_status);

#endif //DELIBIRD_PUBLISHER_H
