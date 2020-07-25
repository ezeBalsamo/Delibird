#include "../include/team_broker_connection_handler.h"
#include "../include/team_manager.h"
#include "../include/team_logs_manager.h"
#include "../include/team_query_performers.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/logger.h"
#include <stdlib.h>
#include <string.h>
#include <team_configuration_manager.h>
#include <commons/string.h>
#include <localized_query_performer.h>
#include <event_notifier.h>

sem_t subscriber_threads_request_sent;

pthread_t appeared_queue_tid;
pthread_t localized_queue_tid;
pthread_t caught_queue_tid;

char* team_process_description;

void* retry_connection_thread(void* connection_information){
    log_initiating_communication_retry_process_with_broker();
    t_connection_information* cast_connection_information = (t_connection_information*) connection_information;
    int reconnection_time_in_seconds = config_get_int_at("TIEMPO_RECONEXION");

    if(reconnect(cast_connection_information) == -1){
        log_failed_retry_of_communication_with_broker();
        sleep_for(reconnection_time_in_seconds);
        retry_connection_thread(connection_information);
    }
    else{
        cast_connection_information -> connection_was_succesful = true;
        log_succesful_retry_of_communication_with_broker();
    }

    return NULL;
}

void execute_retry_connection_strategy(t_connection_information* connection_information){
    log_failed_attempt_to_communicate_with_broker("se procederá a reintentar");

    pthread_t* reconnection_thread = safe_malloc(sizeof(pthread_t));
    consider_as_garbage(reconnection_thread, (void (*)(void *)) safe_thread_pointer_cancel);

    *reconnection_thread = default_safe_thread_create(retry_connection_thread, (void *) connection_information);
    safe_thread_join(*reconnection_thread);

    stop_considering_garbage(reconnection_thread);
}

t_request* subscribe_me_request_for(uint32_t operation_queue){

    t_subscribe_me* subscribe_me = safe_malloc(sizeof(t_subscribe_me));
    subscribe_me -> operation_queue = operation_queue;
    subscribe_me -> process_description = string_duplicate(team_process_description);

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = SUBSCRIBE_ME;
    request -> structure = subscribe_me;
    request -> sanitizer_function = (void (*)(void *)) free_subscribe_me;

    return request;
}

t_connection_information* subscribe_to_broker_queue(void* queue_operation_identifier){

    uint32_t operation_queue = *((uint32_t*) queue_operation_identifier);

    t_request* request = subscribe_me_request_for(operation_queue);

    //Se hace porque cuando se quiere hacer la reconexión piensa que todavía el broker esta levantado!
    //Agradecimientos y reconocimientos: Ing. Barbeito
    sleep_for(1);

    t_connection_information* connection_information = connect_to(broker_ip(), broker_port());

    consider_as_garbage(request, (void (*)(void *)) free_request);
    consider_as_garbage(connection_information, (void (*)(void *)) free_and_close_connection_information);

    if(!connection_information -> connection_was_succesful) {
        execute_retry_connection_strategy(connection_information);
    }

    int ack = serialize_and_send_structure_and_wait_for_ack(request, connection_information -> socket_fd, ack_timeout());

    if(ack == FAILED_ACK){
        stop_considering_garbage(connection_information);
        free_connection_information(connection_information);
        connection_information = subscribe_to_broker_queue(queue_operation_identifier);
    }else{
        log_succesful_suscription_to(main_logger(), operation_queue);
        log_succesful_suscription_to(process_execution_logger(), operation_queue);
    }

    free_request(request);
    stop_considering_garbage(request);

    return connection_information;
}

void resubscribe_to_broker_queue(void* queue_operation_identifier, t_connection_information* connection_information){

    log_broker_disconnection_using(main_logger());
    log_broker_disconnection_using(process_execution_logger());

    t_connection_information* current_active_connection_information = subscribe_to_broker_queue(queue_operation_identifier);
    synchronize_connection_information_closing_old(connection_information, current_active_connection_information);
}

void notify_request_reception(t_request* deserialized_request, int socket_fd){

    t_identified_message* correlative_identified_message = deserialized_request -> structure;
    send_ack_message(correlative_identified_message -> message_id, socket_fd);

    log_request_received(main_logger(), deserialized_request);
    log_request_received(process_execution_logger(), deserialized_request);
}

void consume_messages_considering_reconnections_with(t_connection_information* connection_information,
                                                     void* queue_operation_identifier){

    int current_active_socket_fd = connection_information -> socket_fd;
    t_receive_information* receive_information = receive_structure(current_active_socket_fd);

    if(!receive_information -> receive_was_successful){
        resubscribe_to_broker_queue(queue_operation_identifier, connection_information);
    }else{
        t_request* deserialized_request = deserialize(receive_information -> serialization_information -> serialized_request);

        notify_request_reception(deserialized_request, current_active_socket_fd);
        query_perform(deserialized_request);

        free_request(deserialized_request);
    }

    free_receive_information(receive_information);
}

void* subscriber_thread(void* queue_operation_identifier){

    t_connection_information* connection_information = subscribe_to_broker_queue(queue_operation_identifier);
    safe_sem_post(&subscriber_threads_request_sent);

    while (!is_global_goal_accomplished()){
        consume_messages_considering_reconnections_with(connection_information, queue_operation_identifier);
    }

    return NULL;
}

pthread_t subscribe_to_queue(uint32_t queue_code){
    uint32_t* pokemon_operation_queue_code = safe_malloc(sizeof(uint32_t));
    *pokemon_operation_queue_code = queue_code;

    consider_as_garbage(pokemon_operation_queue_code, free);
    return thread_create(subscriber_thread, (void*) pokemon_operation_queue_code, log_queue_thread_create_error);
}

void subscribe_to_queues(){

    appeared_queue_tid = subscribe_to_queue(APPEARED_POKEMON);
    localized_queue_tid = subscribe_to_queue(LOCALIZED_POKEMON);
    caught_queue_tid = subscribe_to_queue(CAUGHT_POKEMON);

    safe_sem_wait(&subscriber_threads_request_sent);
    safe_sem_wait(&subscriber_threads_request_sent);
    safe_sem_wait(&subscriber_threads_request_sent);
}

void join_to_queues(){

    safe_thread_join(appeared_queue_tid);
    safe_thread_join(localized_queue_tid);
    safe_thread_join(caught_queue_tid);
}

void prepare_get_response(int response_id, char* pokemon_name){

    t_get_response* get_response = safe_malloc(sizeof(t_get_response));
    get_response -> response_id = response_id;
    get_response -> pokemon_name = pokemon_name;

    get_pokemon_sent_successfully(get_response);
}

void apply_default_get_action_for(char* pokemon_name){
    log_no_locations_found_for(pokemon_name);
}

void apply_get_action_when_connection_success(t_request* request, t_connection_information* connection_information, char* pokemon_name){
    int ack =
            serialize_and_send_structure_and_wait_for_ack(request, connection_information -> socket_fd, ack_timeout());

    if(ack == FAILED_ACK){
        apply_default_get_action_for(pokemon_name);
    }
    else{
        prepare_get_response(ack, pokemon_name);
	notify(EXECUTION_CYCLE_CONSUMED);
    }
}

void send_get_pokemon_request_of(t_pokemon_goal* pokemon_goal){

    t_get_pokemon* get_pokemon = safe_malloc(sizeof(t_get_pokemon));
    char* pokemon_name = pokemon_goal -> pokemon_name;
    get_pokemon -> pokemon_name = pokemon_name;

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = GET_POKEMON;
    request -> structure = get_pokemon;
    request -> sanitizer_function = free;

    t_connection_information* connection_information = connect_to(broker_ip(), broker_port());

    if(connection_information -> connection_was_succesful){
        apply_get_action_when_connection_success(request, connection_information, pokemon_name);
    } else{
        apply_default_get_action_for(pokemon_name);
    }

    free_request(request);
    free_and_close_connection_information(connection_information);
}

void initialize_team_process_description(){
    t_list* config_values = all_config_values();
    team_process_description = process_description_for("TEAM", config_values);
    list_destroy_and_destroy_elements(config_values, free);
}

void* initialize_team_broker_connection_handler(){

    safe_sem_initialize(&subscriber_threads_request_sent);
    initialize_team_process_description();

    subscribe_to_queues();
    with_global_goal_do(send_get_pokemon_request_of);
    join_to_queues();

    return NULL;
}

void cancel_all_broker_connection_handler_threads(){
    safe_thread_cancel(appeared_queue_tid);
    safe_thread_cancel(localized_queue_tid);
    safe_thread_cancel(caught_queue_tid);
}

void free_team_broker_connection_handler(){
    free(team_process_description);
    safe_sem_destroy(&subscriber_threads_request_sent);
    cancel_all_broker_connection_handler_threads();
}
