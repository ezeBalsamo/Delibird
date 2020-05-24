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

    int socket_fd = connect_to(ip, port, close_connection_strategy);
    log_successful_connection();
    log_about_to_send_request(request);

    serialize_and_send_structure(request, socket_fd);
    log_request_sent(request);

    //TODO: logica

    free_request(request);

    return NULL;
}

void execute(){

    if(is_subscriber_mode()){
        pthread_t tid = default_safe_thread_create(queue_listener_thread, NULL);
        thread_join(tid);
    }
    else {
        t_request* request = safe_request();
        char* ip = process_ip();
        char* port = process_port();

        int socket_fd = connect_to(ip, port, close_connection_strategy);
        log_successful_connection();
        log_about_to_send_request(request);

        serialize_and_send_structure(request, socket_fd);
        log_request_sent(request);

        free_request(request);
    }
}