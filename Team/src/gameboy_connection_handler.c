#include "../include/gameboy_connection_handler.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/general_logs.h"
#include <stdlib.h>
#include <stdio.h>
#include <appeared_query_performer.h>

t_query_performer* query_performer;

char* port(){
    return config_get_string_at("PUERTO");
}

void* main_thread_handler(void* connection_fd){
    int cast_connection_fd = *((int*) connection_fd);

    t_serialization_information* serialization_information = receive_structure(cast_connection_fd);
    t_request* deserialized_request = deserialize(serialization_information -> serialized_request);

    log_request_received(deserialized_request);

    query_performer -> perform_function (deserialized_request -> structure);

    free_and_close_connection(connection_fd);
    free_serialization_information(serialization_information);
    free(deserialized_request);

    return NULL;
}

void* initialize_gameboy_connection_handler(){
    initialize_appeared_query_performer();
    query_performer = appeared_query_performer();

    start_multithreaded_server(port(), main_thread_handler);

    return NULL;
}