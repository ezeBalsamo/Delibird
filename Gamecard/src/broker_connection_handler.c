#include "../include/gamecard_logs_manager.h"
#include "../include/gamecard_query_performers.h"
#include "../include/gamecard_configuration_manager.h"
#include "../include/broker_connection_handler.h"
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/garbage_collector.h"
#include <stdlib.h>
#include <commons/string.h>

pthread_t new_queue_tid;
pthread_t catch_queue_tid;
pthread_t get_queue_tid;

char* gamecard_process_description;

void sleep_for(int reconnection_time_in_seconds){
    struct timespec deadline;
    deadline.tv_sec = reconnection_time_in_seconds;
    deadline.tv_nsec = 0;
    if(clock_nanosleep(CLOCK_MONOTONIC, 0, &deadline, NULL) != 0){
        log_thread_sleep_time_configuration_error_from_gamecard();
        free_system();
    }
}

void* retry_connection_thread(void* connection_information){
    log_initiating_communication_retry_process_with_broker_from_gamecard();
    int reconnection_time_in_seconds = config_get_int_at("TIEMPO_DE_REINTENTO_CONEXION");

    if(reconnect((t_connection_information*) connection_information) == -1){
        log_failed_retry_of_communication_with_broker_from_gamecard();
        sleep_for(reconnection_time_in_seconds);
        retry_connection_thread(connection_information);
    }
    else{
        log_succesful_retry_of_communication_with_broker_from_gamecard();
    }

    return NULL;
}

void execute_retry_connection_strategy(t_connection_information* connection_information){
    log_failed_attempt_to_communicate_with_broker_from_gamecard("se procederá a reintentar");

    pthread_t* reconnection_thread = safe_malloc(sizeof(pthread_t));
    consider_as_garbage(reconnection_thread, (void (*)(void *)) safe_thread_pointer_cancel);

    *reconnection_thread = default_safe_thread_create(retry_connection_thread, (void *) connection_information);
    safe_thread_join(*reconnection_thread);
}

t_request* subscribe_me_request_for(uint32_t operation_queue){

    t_subscribe_me* subscribe_me = safe_malloc(sizeof(t_subscribe_me));
    subscribe_me -> operation_queue = operation_queue;
    subscribe_me -> process_description = string_duplicate(gamecard_process_description);

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = SUBSCRIBE_ME;
    request -> structure = subscribe_me;
    request -> sanitizer_function = (void (*)(void *)) free_subscribe_me;

    return request;
}

void consume_messages_from(t_connection_information* connection_information) {

    //Obtener request desde el socket_fd correspondiente al broker
    t_receive_information *receive_information = receive_structure(connection_information->socket_fd);

    if (!receive_information->receive_was_successful) {
        pthread_t broker_connection_handler_thread = default_safe_thread_create(
                initialize_gamecard_broker_connection_handler, NULL);

        safe_thread_join(broker_connection_handler_thread);
    }
    t_request* deserialized_request = deserialize(receive_information -> serialization_information -> serialized_request);

    //Aviso al Broker la recepcion del mensaje
    t_identified_message* identified_message = deserialized_request -> structure;
    send_ack_message(identified_message -> message_id,connection_information -> socket_fd);

    //Loguear y mostrar por consola mensaje recibido
    log_request_received_with(main_logger(), deserialized_request);

    //Realizar lógica
    //En este punto tambien debo armar un mensaje con el mismo id que me llego para publicar en la cola corresp
    gamecard_query_perform(deserialized_request);

    //Liberar memoria
    free_receive_information(receive_information);
    free_request(deserialized_request);
}

void* subscriber_thread(void* queue_operation_identifier){

    uint32_t operation_queue = *((uint32_t*) queue_operation_identifier);
    free(queue_operation_identifier);

    t_request* request = subscribe_me_request_for(operation_queue);
    t_connection_information* connection_information = connect_to(broker_ip(), broker_port());

    consider_as_garbage(request, (void (*)(void *)) free_request);
    consider_as_garbage(connection_information, (void (*)(void *)) free_and_close_connection_information);

    if(!connection_information -> connection_was_succesful) {//Si la conexión fallo, reintentar
        execute_retry_connection_strategy(connection_information);
    }

    serialize_and_send_structure_and_wait_for_ack(request, connection_information -> socket_fd, ack_timeout());
    log_succesful_suscription_to(operation_queue);

    //TODO ESTE FREE ROMPE TODO
    //free_connection_information(connection_information);
    //stop_considering_garbage(connection_information);

    free_request(request);
    stop_considering_garbage(request);

    while (true) {
        consume_messages_from(connection_information);
    }

    return NULL;
}

pthread_t subscribe_to_queue(uint32_t queue_code){
    uint32_t* pokemon_operation_queue_code = safe_malloc(sizeof(uint32_t));
    *pokemon_operation_queue_code = queue_code;

    return thread_create(subscriber_thread, (void*) pokemon_operation_queue_code, log_queue_thread_create_error_from_gamecard);
}

void subscribe_to_queues(){

    new_queue_tid = subscribe_to_queue(NEW_POKEMON);
    catch_queue_tid = subscribe_to_queue(CATCH_POKEMON);
    get_queue_tid = subscribe_to_queue(GET_POKEMON);

}

void join_to_queues(){

    safe_thread_join(new_queue_tid);
    safe_thread_join(catch_queue_tid);
    safe_thread_join(get_queue_tid);
}

void initialize_gamecard_process_description(){
    t_list* config_values = all_config_values();
    gamecard_process_description = process_description_for("GAMECARD", config_values);
    list_destroy_and_destroy_elements(config_values, free);
}

void* initialize_gamecard_broker_connection_handler(){

    initialize_gamecard_process_description();

    subscribe_to_queues();
    join_to_queues();

    return NULL;
}

void free_gamecard_broker_connection_handler(){
    free(gamecard_process_description);
}