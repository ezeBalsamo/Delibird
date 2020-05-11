#ifndef SOCKET_H
#define SOCKET_H

#include "serialization_interface.h"

void close_connection_strategy(int socket_fd);

int reconnect(int socket_fd);

int listen_at(char* port);

int connect_to(char* ip, char* port, void (*disconnection_strategy) (int));

int accept_incoming_connections_on(int socket_fd);

void send_structure(t_request* request, int socket_fd);

void* receive_structure(int socket_fd);

void multithreaded_server_listening_at(char* port);

#endif //SOCKET_H
