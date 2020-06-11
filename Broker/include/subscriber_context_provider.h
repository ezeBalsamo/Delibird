#ifndef DELIBIRD_SUBSCRIBER_CONTEXT_PROVIDER_H
#define DELIBIRD_SUBSCRIBER_CONTEXT_PROVIDER_H

#include <stdint.h>

typedef struct Subscriber_context{
    uint32_t operation_queue;
    char* process_id;
    int socket_fd;
    uint32_t last_message_id_received;
}t_subscriber_context;

t_subscriber_context* create_subscriber_context(int socket_fd, uint32_t operation_queue, char* process_id);

#endif //DELIBIRD_SUBSCRIBER_CONTEXT_PROVIDER_H