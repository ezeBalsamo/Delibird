#ifndef DELIBIRD_QUEUE_MESSAGE_STATUS_H
#define DELIBIRD_QUEUE_MESSAGE_STATUS_H

#include "../../Utils/include/common_structures.h"

typedef struct MessageStatus {
    t_identified_message* identified_message;
    t_list* subscribers_to_send;
    t_list* subscribers_who_received;
} t_message_status;

t_message_status* create_message_status_for(t_identified_message* identified_message);
void move_subscriber_to_ACK(t_message_status* message_status, int subscriber);
void free_message_status(t_message_status* message_status);

#endif //DELIBIRD_QUEUE_MESSAGE_STATUS_H
