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

void send_structure(t_serialization_information* serialization_information, int socket_fd);

void serialize_and_send_structure(t_request* request, int socket_fd);

void start_multithreaded_server(char* port, void* (*thread_function) (void* thread_argument));

void* receive_structure(int socket_fd);

void close_connection(int socket_fd);

#endif //SOCKET_H
