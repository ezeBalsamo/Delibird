#ifndef DELIBIRD_SUBSCRIBER_CONTEXT_PROVIDER_H
#define DELIBIRD_SUBSCRIBER_CONTEXT_PROVIDER_H

#include "../../Utils/include/common_structures.h"

typedef struct Subscriber_context{
    uint32_t operation_queue;
    char* process_description;
    int socket_fd;
    uint32_t last_message_id_received;
    bool stable_connection;
}t_subscriber_context;

t_subscriber_context* create_subscriber_context(int socket_fd, t_subscribe_me* subscribe_me);
bool has_stable_connection(t_subscriber_context* subscriber_context);
void set_inestable_connection(t_subscriber_context* subscriber_context);

#endif //DELIBIRD_SUBSCRIBER_CONTEXT_PROVIDER_H