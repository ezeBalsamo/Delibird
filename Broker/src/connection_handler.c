#include <stdlib.h>
#include <message_role_identifier.h>
#include <semaphore.h>
#include "connection_handler.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/socket.h"
#include "../include/broker_logs_manager.h"

uint32_t message_id = 1;
sem_t mutex_id;

char* port(){
    return config_get_string_at("PUERTO_BROKER");
}

uint32_t get_message_id(){
    return message_id;
}

void update_message_id(){
    message_id++;
}

sem_t* get_mutex_id(){
    return &mutex_id;
}

void* main_thread_handler(void* connection_fd){
    int cast_connection_fd = *((int*) connection_fd);
    t_serialization_information* serialization_information = receive_structure(cast_connection_fd);
    log_succesful_connection_of_a_process();

    void* serialized_request = serialization_information -> serialized_request;
    t_request* request = deserialize(serialized_request);
    log_structure_received(serialized_request);

    t_connection_request* connection_request = create_connection_request(cast_connection_fd, request);

    message_role_identifier(connection_request);

    free(serialization_information); // no libero con free_serialization_information porque la serialized_request se guarda en colas para reenviarse
    free(connection_request); // misma historia que arriba

    return NULL;
}

void* initialize_connection_handler(){
    log_server_initial_status();
    sem_init(&mutex_id, false, 1);
    start_multithreaded_server(port(), main_thread_handler);

    return NULL;

}