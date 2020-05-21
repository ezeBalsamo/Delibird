#include "../include/gameboy_connection_handler.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/configuration_manager.h"
#include <stdlib.h>

char* port(){
    return config_get_string_at("PUERTO");
}

void* main_thread_handler(void* connection_fd){
    int cast_connection_fd = *((int*) connection_fd);
    t_serialization_information* serialization_information = receive_structure(cast_connection_fd);
    void* serialized_request = serialization_information -> serialized_request;

    void* serialized_structure = deserialize(serialized_request);

    free_and_close_connection(connection_fd);
    free(serialized_request);

    return serialized_structure;
}

void* initialize_gameboy_connection_handler(){
    start_multithreaded_server(port(), main_thread_handler);

    return NULL;
}