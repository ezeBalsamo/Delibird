#include "../include/broker_deserialization.h"
#include "../../Utils/include/operations_information.h"
#include <stdlib.h>
#include <string.h>
#include "../include/queue_message_manager.h"

void* deserialize(void* serialized_request){
    uint32_t operation;
    void* serialized_structure;
    uint32_t serialized_structure_size;

    int offset = 0;

    operation = get_operation_from(serialized_request);
    offset += sizeof(uint32_t);
    memcpy(&serialized_structure_size, serialized_request + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    serialized_structure = malloc(serialized_structure_size);
    memcpy(serialized_structure, serialized_request + offset, serialized_structure_size);

    t_operation_information* operation_information = operation_information_with_code(operation);

    return (*(operation_information -> deserialize_function)) (serialized_structure);
}

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
