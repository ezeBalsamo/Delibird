#include "../include/team_gameboy_connection_handler.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/general_logs.h"
#include <appeared_query_performer.h>
#include <stdlib.h>

t_query_performer* query_performer;

char* own_port(){
    return config_get_string_at("PUERTO");
}

void* main_thread_handler(void* connection_fd){
    int cast_connection_fd = *((int*) connection_fd);

    t_receive_information* receive_information  = receive_structure(cast_connection_fd);

    if(receive_information -> receive_was_successful){

        t_request* deserialized_request = deserialize(receive_information -> serialization_information -> serialized_request);
        t_identified_message* correlative_identified_message = deserialized_request -> structure;
        send_ack_message(correlative_identified_message -> message_id, cast_connection_fd);

        log_request_received(deserialized_request);

        query_performer -> perform_function (deserialized_request -> structure);

        free_request(deserialized_request);
        free_and_close_connection(connection_fd);
    }
    else{
        free(connection_fd);
    }

    free_receive_information(receive_information);

    return NULL;
}

void* initialize_team_gameboy_connection_handler(){

    query_performer = appeared_query_performer();
    start_multithreaded_server(own_port(), main_thread_handler);

    return NULL;
}

void free_team_gameboy_connection_handler(){
    free_multithreaded_server();
}