#ifndef DELIBIRD_PUBLISH_MESSAGE_MODE_H
#define DELIBIRD_PUBLISH_MESSAGE_MODE_H

#include "../../Utils/include/common_structures.h"
#include "message_role_identifier.h"

typedef struct MessageStatus {
    t_identified_message* identified_message;
    t_list* subscribers_to_send;
    t_list* subscribers_who_received;
} t_message_status;

t_message_role_identifier* publish_message_mode_function();

void initialize_publish_message_mode();

void free_message_status(t_message_status* message_status);

#endif //DELIBIRD_PUBLISH_MESSAGE_MODE_H
