#include "../include/team_gameboy_connection_handler.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <appeared_query_performer.h>

t_query_performer* query_performer;

char* own_port(){
    return config_get_string_at("PUERTO");
}

void* main_thread_handler(void* connection_fd){
    int cast_connection_fd = *((int*) connection_fd);

    t_serialization_information* serialization_information = receive_structure(cast_connection_fd);
    t_request* deserialized_request = deserialize(serialization_information -> serialized_request);

    t_identified_message* correlative_identified_message = deserialized_request -> structure;
    send_ack_message(correlative_identified_message -> message_id, cast_connection_fd);

    log_request_received_with(main_logger(), deserialized_request);

    query_performer -> perform_function (deserialized_request -> structure);

    free_and_close_connection(connection_fd);
    free_serialization_information(serialization_information);
    free_request(deserialized_request);

    return NULL;
}

void* initialize_gameboy_connection_handler(){

    query_performer = appeared_query_performer();
    start_multithreaded_server(own_port(), main_thread_handler);

    return NULL;
}

void free_gameboy_connection_handler(){
    free_multithreaded_server();
}