#include "../include/gameboy_connection_handler.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/pretty_printer.h"
#include <stdlib.h>
#include <stdio.h>

char* port(){
    return config_get_string_at("PUERTO");
}

void* main_thread_handler(void* connection_fd){
    int cast_connection_fd = *((int*) connection_fd);

    t_serialization_information* serialization_information = receive_structure(cast_connection_fd);
    t_request* deserialized_request = deserialize(serialization_information -> serialized_request);

    char* request_as_string = request_pretty_print(deserialized_request);
    printf("%s\n", request_as_string);

    free_and_close_connection(connection_fd);
    free_serialization_information(serialization_information);
    free(deserialized_request);
    free(request_as_string);

    return NULL;
}

void* initialize_gamecard_gameboy_connection_handler(){
    start_multithreaded_server(port(), main_thread_handler);

    return NULL;
}

void free_gameboy_connection_handler(){
    free_multithreaded_server();
}