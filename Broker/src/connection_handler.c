#include <stdlib.h>
#include <queue_message_manager.h>
#include "connection_handler.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/socket.h"
#include "broker_deserialization.h"


char* port(){
    return config_get_string_at("PUERTO_BROKER");
}

void* main_thread_handler(void* connection_fd){
    int cast_connection_fd = *((int*) connection_fd);
    t_serialization_information* serialization_information = receive_structure(cast_connection_fd);
    void* serialized_request = serialization_information -> serialized_request;

    uint32_t operation = get_operation_from(serialized_request);

    t_connection_request* connection_request = create_connection_request(cast_connection_fd, serialized_request);

    push_to_queue(operation, connection_request);
    publish(operation, serialization_information);

//    free_and_close_connection(connection_fd);
    free(serialization_information);
    free(connection_request);

    return NULL;
}

void* initialize_connection_handler(){

    start_multithreaded_server(port(), main_thread_handler);

    return NULL;

}