#include "../include/entry_point_executor.h"
#include "../include/entry_point_connection_builder.h"
#include "../include/entry_point_validator.h"
#include "../include/entry_point_logs_manager.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/logger.h"
#include <signal.h>
#include <stdlib.h>

timer_t timer;

void consume_messages_from(int socket_fd){
    while(true){
        t_receive_information* receive_information = receive_structure(socket_fd);
        if(!receive_information -> receive_was_successful){
            log_broker_disconnection(main_logger());
            free_system();
        }
        t_request* deserialized_request = deserialize(receive_information -> serialization_information -> serialized_request);

        t_identified_message* identified_message = deserialized_request -> structure;
        send_ack_message(identified_message -> message_id, socket_fd);

        log_request_received_with(main_logger(), deserialized_request);
        log_request_received_with(process_execution_logger(), deserialized_request);

        free_receive_information(receive_information);
        free_request(deserialized_request);
    }
}

struct itimerspec itimerspec_for_seconds(int amount_of_seconds){
    struct itimerspec itimerspec;
    itimerspec.it_value.tv_sec = amount_of_seconds;
    itimerspec.it_value.tv_nsec = 0;
    itimerspec.it_interval.tv_sec = amount_of_seconds;
    itimerspec.it_interval.tv_nsec = 0;

    return itimerspec;
}

void set_timer_with(struct itimerspec itimerspec){
    if(timer_settime(timer, 0, &itimerspec, NULL) == -1){
        log_syscall_error("Error en la configuración del tiempo para el timer");
        free_system();
    }
}

void timer_handler(){
    timer_delete(timer);
    free_system();
}

void initialize_timer(){
    handle_signal(SIGALRM, timer_handler);

    if(timer_create(CLOCK_MONOTONIC, NULL, &timer) == -1){
        log_syscall_error("Error en la creación del timer");
        free_system();
    }
}

void start_timer_with_seconds(int amount_of_seconds){
    initialize_timer();
    struct itimerspec itimerspec = itimerspec_for_seconds(amount_of_seconds);
    set_timer_with(itimerspec);
}

void subscriber_mode_execution(){

    t_request* request = safe_request();
    char* ip = process_ip();
    char* port = process_port();

    t_connection_information* connection_information = connect_to(ip, port);

    if (!connection_information -> connection_was_succesful){
        free_request(request);
        close_failed_connection(connection_information);
    }

    consider_as_garbage(connection_information, (void (*)(void *)) free_and_close_connection_information);
    log_successful_connection();
    log_about_to_send_request(request);

    serialize_and_send_structure_and_wait_for_ack(request, connection_information -> socket_fd, 5);

    t_subscribe_me* subscribe_me = request -> structure;
    log_succesful_suscription_to(subscribe_me -> operation_queue);

    log_request_sent(request);
    free_request(request);

    int time_in_seconds = suscriber_mode_time_in_seconds();
    start_timer_with_seconds(time_in_seconds);
    consume_messages_from(connection_information -> socket_fd);
}

void publisher_mode_execution(){
    t_request* request = safe_request();
    char* ip = process_ip();
    char* port = process_port();

    t_connection_information* connection_information = connect_to(ip, port);

    if (!connection_information -> connection_was_succesful){
        free_request(request);
        close_failed_connection(connection_information);
    }

    log_successful_connection();
    log_about_to_send_request(request);

    serialize_and_send_structure(request, connection_information -> socket_fd);
    void* ack = receive_ack_with_timeout_in_seconds(connection_information -> socket_fd, 5);
    log_request_sent(request);
    free(ack);

    free_request(request);
    free_and_close_connection_information(connection_information);
}

void execute(){

    if(is_subscriber_mode()){
        subscriber_mode_execution();
    }
    else {
        publisher_mode_execution();
    }
}