#include <stdlib.h>
#include <message_role_identifier.h>
#include <semaphore.h>
#include <pthread.h>
#include "connection_handler.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/socket.h"
#include "../include/broker_logs_manager.h"

uint32_t message_id = 1;
pthread_mutex_t mutex_id;

char* port(){
    return config_get_string_at("PUERTO_BROKER");
}

uint32_t update_and_get_message_id(){
    pthread_mutex_lock(&mutex_id);
    message_id++;
    pthread_mutex_unlock(&mutex_id);
    return message_id;
}

void* main_thread_handler(void* connection_fd){
    int cast_connection_fd = *((int*) connection_fd);
    t_serialization_information* serialization_information = receive_structure(cast_connection_fd);
    log_succesful_connection_of_a_process();

    t_request* request = deserialize(serialization_information -> serialized_request);
    log_structure_received(request); //todo mejorar y poner nombre de suscriptor.

    t_connection_request* connection_request = create_connection_request(cast_connection_fd, request);

    attend_with_message_role_identifier(connection_request);

    free(serialization_information); // no libero con free_serialization_information porque la serialized_request se guarda en colas para reenviarse
    free(connection_request); // misma historia que arriba

    return NULL;
}

void* initialize_connection_handler(){
    log_server_initial_status();
    pthread_mutex_init(&mutex_id, NULL);
    start_multithreaded_server(port(), main_thread_handler);

    return NULL;

}