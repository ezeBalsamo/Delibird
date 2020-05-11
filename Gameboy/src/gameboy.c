#include "../include/entry_point_connection_builder.h"
#include "../include/entry_point_validator.h"
#include "../include/entry_point_logs_manager.h"
#include "../../Utils/include/socket.h"
#include <stdlib.h>

int main(int arguments_amount, char* arguments[]) {

    initialize_entry_point_logs_manager();
    initialize_entry_point_validator(arguments_amount, arguments);
    initialize_entry_point_connection_builder();
    log_succesful_start_up();

    t_request* request = safe_request();
    char* ip = process_ip();
    char* port = process_port();

    int socket_fd = connect_to(ip, port);
    log_successful_connection();
    log_about_to_send_request(request);
    send_structure(request, socket_fd);
    log_request_sent(request);

    free(request);

    free_entry_point_validator();
    free_entry_point_logs_manager();
    free_entry_point_connection_builder();

    return 0;
}