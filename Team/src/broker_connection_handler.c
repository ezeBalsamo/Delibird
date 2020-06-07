#include "../include/broker_connection_handler.h"
#include "../include/team_manager.h"
#include "../include/team_logs_manager.h"
#include "../include/query_performer.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/free_system.h"
#include "../../Utils/include/general_logs.h"
#include <stdlib.h>
#include <string.h>
#include <team_configuration_manager.h>

sem_t subscriber_threads_request_sent;

pthread_t appeared_queue_tid;
pthread_t localized_queue_tid;
pthread_t caught_queue_tid;

void sleep_for(int reconnection_time_in_seconds){
    struct timespec deadline;
    deadline.tv_sec = reconnection_time_in_seconds;
    deadline.tv_nsec = 0;
    if(clock_nanosleep(CLOCK_MONOTONIC, 0, &deadline, NULL) != 0){
        log_thread_sleep_time_configuration_error();
        free_system();
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

void execute_retry_connection_strategy(t_connection_information* connection_information){
    log_failed_attempt_to_communicate_with_broker();
    pthread_t reconnection_thread = default_safe_thread_create(retry_connection_thread, (void *) connection_information);
    thread_join(reconnection_thread);
}

void* subscriber_thread(void* queue_operation_identifier){

    t_subscribe_me* subscribe_me = safe_malloc(sizeof(t_subscribe_me));
    subscribe_me -> operation_queue = *((uint32_t*) queue_operation_identifier);

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = SUBSCRIBE_ME;
    request -> structure = subscribe_me;
    request -> sanitizer_function = free;

    t_connection_information* connection_information = connect_to(broker_ip(), broker_port());

    if(!connection_information -> connection_was_succesful) {
        execute_retry_connection_strategy(connection_information);
    }
    else {
        send_structure(request, connection_information -> socket_fd);
        sem_post(&subscriber_threads_request_sent);
        request -> sanitizer_function (request);

        while (true) {
            t_serialization_information* serialization_information = receive_structure(connection_information -> socket_fd);
            t_request* deserialized_request = deserialize(serialization_information -> serialized_request);

            log_request_received(deserialized_request);

            query_perform(deserialized_request);

            free_serialization_information(serialization_information);
            free_request(deserialized_request);
        }
    }

    return NULL;
}

pthread_t subscribe_to_queue(uint32_t queue_code){
    uint32_t* pokemon_operation_queue_code = safe_malloc(sizeof(uint32_t));
    *pokemon_operation_queue_code = queue_code;

    return thread_create(subscriber_thread, (void*) pokemon_operation_queue_code, log_queue_thread_create_error);
}

void subscribe_to_queues(){

    appeared_queue_tid = subscribe_to_queue(APPEARED_POKEMON);
    localized_queue_tid = subscribe_to_queue(LOCALIZED_POKEMON);
    caught_queue_tid = subscribe_to_queue(CAUGHT_POKEMON);

    sem_wait(&subscriber_threads_request_sent);
    sem_wait(&subscriber_threads_request_sent);
    sem_wait(&subscriber_threads_request_sent);
}

void join_to_queues(){

    thread_join(appeared_queue_tid);
    thread_join(localized_queue_tid);
    thread_join(caught_queue_tid);
}

void send_get_pokemon_request_of(t_pokemon_goal* pokemon_goal){

    t_get_pokemon* get_pokemon = safe_malloc(sizeof(t_get_pokemon));
    get_pokemon -> pokemon_name = pokemon_goal -> pokemon_name;

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = GET_POKEMON;
    request -> structure = get_pokemon;
    request -> sanitizer_function = free;

    t_connection_information* connection_information = connect_to(broker_ip(), broker_port());

    if(connection_information -> connection_was_succesful){
        send_structure(request, connection_information -> socket_fd);
        request -> sanitizer_function (request);
    } else{
        log_no_locations_found_for(pokemon_goal -> pokemon_name);
    }
    free_and_close_connection_information(connection_information);
}

void* initialize_broker_connection_handler(){

    sem_initialize(&subscriber_threads_request_sent);

    subscribe_to_queues();
    with_global_goal_do(send_get_pokemon_request_of);
    join_to_queues();
    return NULL;
}