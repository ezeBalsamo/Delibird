#include "../include/entry_point_connection_builder.h"
#include "../include/entry_point_validator.h"
#include "../../Utils/include/socket.h"
#include <stdlib.h>

int main(int arguments_amount, char* arguments[]) {

    initialize_entry_point_validator(arguments_amount, arguments);
    initialize_entry_point_connection_builder();

    t_request* request = safe_request();
    char* ip = process_ip();
    char* port = process_port();

    int socket_fd = connect_to(ip, port);
    send_structure(request, socket_fd);

    free(ip);
    free(port);
    free_request(request);
    free_entry_point_validator();
    free_entry_point_connection_builder();

    return 0;
}