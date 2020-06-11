#include "subscriber_context_provider.h"
#include "../../Utils/include/common_structures.h"

t_subscriber_context* create_subscriber_context(int socket_fd, uint32_t operation_queue, char* process_id){

    t_subscriber_context* subscriber_context = safe_malloc(sizeof(t_subscriber_context));
    subscriber_context -> operation_queue = operation_queue;
    subscriber_context -> socket_fd = socket_fd;
    subscriber_context -> process_id = process_id;
    subscriber_context -> last_message_id_received = 0; //lo inicializo en 0 por las dudas de que hayan problemas si no lo inicializo ahora.

    return  subscriber_context;
}
