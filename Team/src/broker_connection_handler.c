#include "../include/broker_connection_handler.h"
#include "../include/team_manager.h"
#include "../include/team_logs_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/pthread_wrapper.h"
#include <commons/process.h>
#include <stdlib.h>
#include <string.h>

int appeared_queue_socket_fd;
int caught_queue_socket_fd;
int localized_queue_socket_fd;

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

void* retry_connection_thread(void* socket_fd){
    log_initiating_communication_retry_process_with_broker();
    int reconnection_time_in_seconds = config_get_int_at("TIEMPO_RECONEXION");

    if(reconnect(*((int*) socket_fd)) == -1){
        log_failed_retry_of_communication_with_broker();
        sleep_for(reconnection_time_in_seconds);
        retry_connection_thread(socket_fd);
    }
    else{
        log_succesful_retry_of_communication_with_broker();
    }
}

void reconnection_strategy(int socket_fd){
    log_failed_attempt_to_communicate_with_broker();
    pthread_t reconnection_thread = thread_create(retry_connection_thread, (void*) &socket_fd, default_thread_create_error_response_strategy);
    thread_join(reconnection_thread);
}

t_request* handshake_request_for(uint32_t queue_operation_identifier){

    uint32_t process_id = process_getpid();
    void* structure = malloc(sizeof(uint32_t) * 2);
    uint32_t offset = 0;
    memcpy(structure + offset, &process_id, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(structure + offset, &queue_operation_identifier, sizeof(uint32_t));

    t_request* request = malloc(sizeof(t_request));
    request -> operation = SUSCRIPTOR;
    request -> structure = structure;

    return request;
}

void subscribe_to_queue(int* socket_fd, uint32_t queue_operation_identifier){

    t_request* request = handshake_request_for(queue_operation_identifier);
    *socket_fd = connect_to(broker_ip, broker_port, reconnection_strategy);
    send_structure(request, *socket_fd);
}

void subscribe_to_queues(){

    subscribe_to_queue(&appeared_queue_socket_fd, APPEARED_POKEMON);
    subscribe_to_queue(&localized_queue_socket_fd, LOCALIZED_POKEMON);
    subscribe_to_queue(&caught_queue_socket_fd, CAUGHT_POKEMON);
}

void send_get_pokemon_request_of(t_pokemon_goal* pokemon_goal){

    t_request* request = malloc(sizeof(t_request));
    request -> operation = GET_POKEMON;
    request -> structure = (void*) (pokemon_goal -> pokemon_name);

    int socket_fd = connect_to(broker_ip, broker_port, reconnection_strategy);
    send_structure(request, socket_fd);
    free(request);
}

void* initialize_broker_connection_handler(){
    broker_ip = config_get_string_at("IP_BROKER");
    broker_port = config_get_string_at("PUERTO_BROKER");

    subscribe_to_queues();
    with_global_goal_do(send_get_pokemon_request_of);

    return NULL;
}