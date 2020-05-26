#include "../include/entry_point_executor.h"
#include "../include/entry_point_connection_builder.h"
#include "../include/entry_point_validator.h"
#include "../include/entry_point_logs_manager.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/pthread_wrapper.h"
#include <stdlib.h>

void* queue_listener_thread(){

    t_request* request = safe_request();
    char* ip = process_ip();
    char* port = process_port();

    t_connection_information* connection_information = connect_to(ip, port);

    if (!connection_information -> connection_was_succesful){
        close_and_exit_failed_connection(connection_information);
    }
    else{
        log_successful_connection();
        log_about_to_send_request(request);

        serialize_and_send_structure(request, connection_information -> socket_fd);
        log_request_sent(request);

        //TODO: logica

        free_request(request);
    }
    return NULL;
}

void subscriber_mode_execution(){
    pthread_t tid = default_safe_thread_create(queue_listener_thread, NULL);
    thread_join(tid);
}

void publisher_mode_execution(){
    t_request* request = safe_request();
    char* ip = process_ip();
    char* port = process_port();

    t_connection_information* connection_information = connect_to(ip, port);

    if (!connection_information -> connection_was_succesful){
        close_and_exit_failed_connection(connection_information);
    }
    else{
        log_successful_connection();
        log_about_to_send_request(request);

        serialize_and_send_structure(request, connection_information -> socket_fd);
        log_request_sent(request);

        free_and_close_connection_information(connection_information);

    }
    //en cualquiera de los dos casos quiero que se libere la request.
    free_request(request);
}

void execute(){

    if(is_subscriber_mode()){
        subscriber_mode_execution();
    }
    else {
        publisher_mode_execution();
    }
}