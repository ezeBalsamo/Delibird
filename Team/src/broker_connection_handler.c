#include "../include/broker_connection_handler.h"
#include "../include/team_manager.h"
#include "../include/team_logs_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/pthread_wrapper.h"
#include <stdlib.h>
#include <string.h>

char* broker_ip;
char* broker_port;

void sleep_for(int reconnection_time_in_seconds){
    struct timespec deadline;
    deadline.tv_sec = reconnection_time_in_seconds;
    deadline.tv_nsec = 0;
    if(clock_nanosleep(CLOCK_MONOTONIC, 0, &deadline, NULL) != 0){
        log_thread_sleep_time_configuration_error();
    }
}

void* retry_connection_thread(void* connection_information){
    log_initiating_communication_retry_process_with_broker();
    int reconnection_time_in_seconds = config_get_int_at("TIEMPO_RECONEXION");

    if(reconnect((t_connection_information*) connection_information) == -1){
        log_failed_retry_of_communication_with_broker();
        sleep_for(reconnection_time_in_seconds);
        retry_connection_thread(connection_information);
    }
    else{
        log_succesful_retry_of_communication_with_broker();
    }

    return NULL;
}

void reconnection_strategy(t_connection_information* connection_information){
    log_failed_attempt_to_communicate_with_broker();
    pthread_t reconnection_thread = thread_create(retry_connection_thread, (void *) connection_information,
                                                  default_thread_create_error_response);
    thread_join(reconnection_thread);
}

void* subscriber_thread(void* queue_operation_identifier){
    t_request* request = malloc(sizeof(t_request));
    request -> operation = SUBSCRIBE_ME;
    request -> structure = queue_operation_identifier;

    int socket_fd = connect_to(broker_ip, broker_port, reconnection_strategy);
    serialize_and_send_structure(request, socket_fd);

    while(true){
        t_serialization_information* serialization_information = receive_structure(socket_fd);

        deserialize(serialization_information -> serialized_request);
    }
    //TODO: Lógica para escuchar
}

void subscribe_to_queues(){
    uint32_t appeared_queue_identifier = APPEARED_POKEMON;
    uint32_t caught_queue_identifier = CAUGHT_POKEMON;
    uint32_t localized_queue_identifier = LOCALIZED_POKEMON;

    thread_create(subscriber_thread, (void*) &appeared_queue_identifier, log_queue_thread_create_error);
    thread_create(subscriber_thread, (void*) &caught_queue_identifier, log_queue_thread_create_error);
    thread_create(subscriber_thread, (void*) &localized_queue_identifier, log_queue_thread_create_error);
}

void send_get_pokemon_request_of(t_pokemon_goal* pokemon_goal){

    t_request* request = malloc(sizeof(t_request));
    request -> operation = GET_POKEMON;
    request -> structure = (void*) (pokemon_goal -> pokemon_name);

    int socket_fd = connect_to(broker_ip, broker_port, reconnection_strategy);
    serialize_and_send_structure(request, socket_fd);
    free(request);
}

void* initialize_broker_connection_handler(){
    broker_ip = config_get_string_at("IP_BROKER");
    broker_port = config_get_string_at("PUERTO_BROKER");

    subscribe_to_queues();
//    with_global_goal_do(send_get_pokemon_request_of);

    return NULL;
}