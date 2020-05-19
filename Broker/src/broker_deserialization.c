#include "../include/broker_deserialization.h"
#include "../../Utils/include/operations_information.h"
#include <stdlib.h>
#include <string.h>
#include "../include/queue_message_manager.h"

void* deserialize(void* serialized_request){
    uint32_t operation;
    int socket_fd;
    void* serialized_structure;
    uint32_t serialized_structure_size;

    int offset = 0;

    memcpy(&operation, serialized_request + offset, sizeof(uint32_t)); //me hace ruido que aca se obtenga la operation.
    offset += sizeof(uint32_t);
    memcpy(&socket_fd, serialized_request + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&serialized_structure_size, serialized_request + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    serialized_structure = malloc(serialized_structure_size);
    memcpy(serialized_structure, serialized_request + offset, serialized_structure_size);

    t_operation_information* operation_information = operation_information_with_code(operation);

    t_connection_request * connection_request = malloc(sizeof(t_connection_request));
    connection_request -> socket_fd = socket_fd;
    connection_request -> serialized_request = serialized_structure; //no hay problema despues para deserializar mas adelante...
    // tenes la capacidad de saber cual es la operacion a la cual estas tratando porque la vas a tratar desde la cola.

    move_to_queue(operation, connection_request);

    return (*(operation_information -> deserialize_function)) (serialized_structure);
}