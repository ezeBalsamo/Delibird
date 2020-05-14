#ifndef SOCKET_H
#define SOCKET_H

#include "serialization_interface.h"

typedef struct Connection_Information{
    int socket_fd;
    struct addrinfo* address_interface;
}t_connection_information;

void close_connection_strategy(t_connection_information* connection_information);

int reconnect(t_connection_information* connection_information);

int listen_at(char* port);

int connect_to(char* ip, char* port, void (*disconnection_strategy) (t_connection_information*));

int accept_incoming_connections_on(int socket_fd);

void send_structure(t_request* request, int socket_fd);

void* receive_structure(int socket_fd);

void multithreaded_server_listening_at(char* port);

#endif //SOCKET_H
