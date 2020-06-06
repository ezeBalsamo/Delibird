#ifndef SOCKET_H
#define SOCKET_H

#include "serialization_interface.h"

typedef struct Connection_Information{
    int socket_fd;
    struct addrinfo* address_interface;
    bool connection_was_succesful;
}t_connection_information;

void close_failed_connection(t_connection_information* connection_information);

int reconnect(t_connection_information* connection_information);

int listen_at(char* port);

t_connection_information* connect_to(char* ip, char* port);

int accept_incoming_connections_on(int socket_fd);

void send_structure(t_serialization_information* serialization_information, int socket_fd);

void serialize_and_send_structure(t_request* request, int socket_fd);

void send_ack_message(uint32_t message_id, int socket_fd);

void* receive_ack_message(void* socket_fd);

void start_multithreaded_server(char* port, void* (*thread_function) (void* thread_argument));

t_serialization_information* receive_structure(int socket_fd);

void free_and_close_connection(void* socket_fd);

void free_and_close_connection_information(t_connection_information* connection_information);

#endif //SOCKET_H
