#include <messages_roles.h>
#include <semaphore.h>
#include <stdlib.h>
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/socket.h"
#include "../include/broker_logs_manager.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/operation_deserialization.h"

uint32_t message_id = 0;

char* port(){
    return config_get_string_at("PUERTO_BROKER");
}

uint32_t update_and_get_message_id(){
    message_id++;
    return message_id;
}

t_connection_deserialization_information* create_connection_deserialization_information(int connection_fd,
                                                                                        t_deserialization_information* deserialization_information){

    t_connection_deserialization_information* connection_deserialization_information =
            safe_malloc(sizeof(t_connection_deserialization_information));

    connection_deserialization_information -> socket_fd = connection_fd;
    connection_deserialization_information -> deserialization_information = deserialization_information;

    return connection_deserialization_information;
}

void* main_thread_handler(void* connection_fd){
    int cast_connection_fd = *((int*) connection_fd);
    t_receive_information* receive_information = receive_structure(cast_connection_fd);
    log_succesful_connection_of_a_process();

    if(receive_information -> receive_was_successful){
        consider_as_garbage(receive_information, (void (*)(void *)) free_receive_information);

        t_deserialization_information* deserialization_information =
                deserialization_information_of(receive_information -> serialization_information -> serialized_request);

        log_received_message_of(deserialization_information -> operation);

        t_connection_deserialization_information* connection_deserialization_information =
                create_connection_deserialization_information(cast_connection_fd, deserialization_information);

        attend_with_message_role(connection_deserialization_information);
    }else{
        free_receive_information(receive_information);
    }

    return NULL;
}

void* initialize_connection_handler(){

    log_server_initial_status();
    start_multithreaded_server(port(), main_thread_handler);

    return NULL;
}

void free_connection_handler(){
    free_multithreaded_server();
}

void free_connection_deserialization_information(t_connection_deserialization_information* connection_deserialization_information){
    free_deserialization_information(connection_deserialization_information -> deserialization_information);
    free(connection_deserialization_information);
}