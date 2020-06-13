#ifndef SOCKET_H
#define SOCKET_H

#include "serialization_interface.h"

#define THREAD_POOL_SIZE 25
#define FAILED_ACK 0

typedef struct Connection_Information{
    int socket_fd;
    struct addrinfo* address_interface;
    bool connection_was_succesful;
}t_connection_information;

void close_failed_connection(t_connection_information* connection_information);

int reconnect(t_connection_information* connection_information);

t_connection_information* connect_to(char* ip, char* port);

void send_serialized_structure(t_serialization_information* serialization_information, int socket_fd);

void send_structure(t_request* request, int socket_fd);

void send_ack_message(uint32_t message_id, int socket_fd);

void* receive_ack_with_timeout_in_seconds(int socket_fd, int timeout_in_seconds);

t_serialization_information* receive_structure(int socket_fd);

void start_multithreaded_server(char* port, void* (*thread_function) (void* thread_argument));

void free_and_close_connection(void* socket_fd);
void free_connection_information(t_connection_information* connection_information);
void free_and_close_connection_information(t_connection_information* connection_information);
void free_multithreaded_server();

#endif //SOCKET_H
