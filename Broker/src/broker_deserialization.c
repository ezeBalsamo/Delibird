#include "../include/broker_deserialization.h"
#include <stdlib.h>
#include <string.h>
#include "../include/queue_message_manager.h"

t_connection_request* create_connection_request(int connection_fd, void* serialized_request){
    t_connection_request* connection_request = malloc(sizeof(t_connection_request));
    connection_request -> socket_fd = connection_fd;
    connection_request -> serialized_request = serialized_request;

    return connection_request;
}

uint32_t get_operation_from(void* serialized_request){
    uint32_t operation;

    memcpy(&operation, serialized_request, sizeof(uint32_t));

    return operation;
}
