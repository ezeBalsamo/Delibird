#ifndef SOCKET_H
#define SOCKET_H

#include "serialization.h"

int listen_at(char* port);

int connect_to(char* ip, char* port);

int accept_incoming_connections_on(int socket_fd);

void send_structure(request* request, int socket_fd);

void* receive_structure(int socket_fd);

void multithreaded_server_listening_at(char* port);

#endif //SOCKET_H
