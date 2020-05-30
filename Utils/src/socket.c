#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <pthread.h>
#include <stdint.h>
#include "../include/socket.h"
#include <commons/collections/queue.h>
#include <commons/process.h>
#include <pthread_wrapper.h>
#include <semaphore.h>

#define THREAD_POOL_SIZE 25

pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t client_sockets_amount_in_queue;
t_queue* queue;

int found_local_ip_address_in(struct ifaddrs* interface_address){

    return (interface_address -> ifa_addr -> sa_family == AF_INET) &&
           (strcmp(interface_address -> ifa_name, "lo") != 0) &&
           (strcmp(interface_address -> ifa_name, "docker0") != 0);
}

char* get_local_ip_address() {
    struct ifaddrs* interface_addresses;
    struct sockaddr_in* af_inet_address_interface;
    char* local_ip_address = NULL;

    if (getifaddrs(&interface_addresses) == -1) {
        perror("getifaddrs error");
        exit(EXIT_FAILURE);
    }

    while(interface_addresses != NULL) {

        if (found_local_ip_address_in(interface_addresses)) {
            af_inet_address_interface = (struct sockaddr_in*) interface_addresses -> ifa_addr;
            local_ip_address = inet_ntoa(af_inet_address_interface -> sin_addr);
        }
        interface_addresses = interface_addresses->ifa_next;
    }

    freeifaddrs(interface_addresses);

    if (local_ip_address == NULL) {
        printf("Failed getting local ip address\n");
        exit(EXIT_FAILURE);
    }

    return local_ip_address;
}

struct addrinfo* build_address_interface(char* ip, char* port){
    int addrinfo_status;
    struct addrinfo hints;
    struct addrinfo* address_interface;

    memset(&hints, 0, sizeof(hints)); // initialize struct
    hints.ai_family = AF_INET;           // set IPv4
    hints.ai_socktype = SOCK_STREAM;     // TCP stream sockets

    if ((addrinfo_status = getaddrinfo(ip, port, &hints, &address_interface) != 0)) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(addrinfo_status));
        exit(EXIT_FAILURE);
    }

    return address_interface;
}

int get_socket_fd_using(struct addrinfo* address_interface){
    int socket_fd;

    if ((socket_fd = socket(address_interface -> ai_family,
                            address_interface -> ai_socktype,
                            address_interface -> ai_protocol)) == -1) {
        perror("socket error");
        freeaddrinfo(address_interface);
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}

void allow_port_reusability(int socket_fd, struct addrinfo* address_interface){
    bool reuse_ports = true;

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_ports, sizeof(int)) == -1) {
        close(socket_fd);
        perror("setsockopt error");
        freeaddrinfo(address_interface);
        exit(EXIT_FAILURE);
    }
}

void bind_port_to_socket(int socket_fd, struct addrinfo* address_interface){

    if (bind(socket_fd, address_interface -> ai_addr, address_interface -> ai_addrlen) == -1) {
        close(socket_fd);
        perror("bind error");
        fprintf(stderr, "server failed to bind\n");
        freeaddrinfo(address_interface);
        exit(EXIT_FAILURE);
    }
}

void listen_with(int socket_fd){

    if (listen(socket_fd, SOMAXCONN) == -1) {
        close(socket_fd);
        perror("listen error");
        exit(EXIT_FAILURE);
    }
}


int accept_incoming_connections_on(int socket_fd){
    int connection_fd;
    struct sockaddr_storage client_address;
    socklen_t address_size;

    address_size = sizeof client_address;
    connection_fd = accept(socket_fd, (struct sockaddr *) &client_address, &address_size);
    if (connection_fd == -1) {
        perror("accept error");
        exit(EXIT_FAILURE);
    }

    return connection_fd;
}

int listen_at(char* port) {

    char* ip = get_local_ip_address();
    struct addrinfo* address_interface = build_address_interface(ip, port);

    int socket_fd = get_socket_fd_using(address_interface);
    allow_port_reusability(socket_fd, address_interface);
    bind_port_to_socket(socket_fd, address_interface);

    freeaddrinfo(address_interface);

    listen_with(socket_fd);

    return socket_fd;
}

void free_connection_information(t_connection_information* connection_information){
    freeaddrinfo(connection_information -> address_interface);
    free(connection_information);
}

void free_and_close_connection_information(t_connection_information* connection_information){
    close(connection_information -> socket_fd);
    free_connection_information(connection_information);
}

int reconnect(t_connection_information* connection_information){
    return connect(connection_information -> socket_fd,
                   connection_information -> address_interface -> ai_addr,
                   connection_information -> address_interface -> ai_addrlen);
}

void close_failed_connection(t_connection_information* connection_information){
    perror("connect error");
    free_and_close_connection_information(connection_information);
}

int establish_connection(int socket_fd, struct addrinfo* address_interface){

    if (connect(socket_fd, address_interface -> ai_addr, address_interface -> ai_addrlen) == -1) {
        return false;
    }
    return true;
}

t_connection_information* connect_to(char* ip, char* port) {

    struct addrinfo* address_interface = build_address_interface(ip, port);
    int socket_fd = get_socket_fd_using(address_interface);

    bool connection_was_succesful = establish_connection(socket_fd, address_interface);

    t_connection_information* connection_information = malloc(sizeof(t_connection_information));
    connection_information -> socket_fd = socket_fd;
    connection_information -> address_interface = address_interface;
    connection_information -> connection_was_succesful = connection_was_succesful;

    return connection_information;
}

void send_all(int socket_fd, void* serialized_request, int amount_of_bytes){

    int sent_bytes = 0;
    int left_bytes = amount_of_bytes;
    int partially_sent_bytes;

    while(sent_bytes < amount_of_bytes){
        partially_sent_bytes = send(socket_fd, serialized_request + sent_bytes, left_bytes, 0);

        if(partially_sent_bytes == -1){
            perror("send_all error");
            printf("only %d bytes sent.\n", sent_bytes);
            close(socket_fd);
            exit(EXIT_FAILURE);
        }
        sent_bytes += partially_sent_bytes;
        left_bytes -= partially_sent_bytes;
    }
}

void send_structure(t_serialization_information* serialization_information, int socket_fd) {

    uint32_t total_amount_of_bytes =
            serialization_information -> amount_of_bytes    // amount_of_bytes_of_request
            + sizeof(uint32_t);                             // total_amount

    void* serialized_request = malloc(total_amount_of_bytes);

    memcpy(serialized_request,
            &(serialization_information -> amount_of_bytes), sizeof(uint32_t));

    memmove(serialized_request + sizeof(uint32_t),
            serialization_information -> serialized_request,
            serialization_information -> amount_of_bytes);

    send_all(socket_fd, serialized_request, total_amount_of_bytes);
    free(serialized_request);
}

void serialize_and_send_structure(t_request* request, int socket_fd){

    t_serialization_information* request_serialization_information = serialize(request);
    send_structure(request_serialization_information, socket_fd);
    free_serialization_information(request_serialization_information);
}

t_serialization_information* receive_structure(int socket_fd){

    void* serialized_request;
    uint32_t amount_of_bytes_of_request;

    if(recv(socket_fd, &amount_of_bytes_of_request, sizeof(uint32_t), MSG_WAITALL) == -1){
        perror("recv amount of bytes error");
        close(socket_fd);
    }

    serialized_request = malloc(amount_of_bytes_of_request);

    if(recv(socket_fd, serialized_request, amount_of_bytes_of_request, MSG_WAITALL) == -1){
        perror("recv serialized structure error");
        close(socket_fd);
    }

    t_serialization_information* serialization_information = malloc(sizeof(t_serialization_information));
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    serialization_information -> serialized_request = serialized_request;

    return serialization_information;
}

void start_multithreaded_server(char* port, void* (*handle_connection_function) (void*)){
    queue = queue_create();
    sem_init(&client_sockets_amount_in_queue, false, 0);

    void* _thread_function(){
        while(true){
            sem_wait(&client_sockets_amount_in_queue);
            pthread_mutex_lock(&queue_mutex);
            void* client_socket_fd = queue_pop(queue);
            pthread_mutex_unlock(&queue_mutex);

            (*handle_connection_function) (client_socket_fd);
        }

        return NULL;
    }

    for(int i = 0; i < THREAD_POOL_SIZE; i++){
        if(pthread_create(&thread_pool[i], NULL, _thread_function, NULL) != 0){
            printf("An error occurred while creating a new thread for attending an incoming connection\n");
            exit(EXIT_FAILURE);
        }
    }

    int server_socket_fd = listen_at(port);

    while(true){
        int* client_socket_fd = malloc(sizeof(int));
        *client_socket_fd = accept_incoming_connections_on(server_socket_fd);

        pthread_mutex_lock(&queue_mutex);
        queue_push(queue, (void*) client_socket_fd);
        sem_post(&client_sockets_amount_in_queue);
        pthread_mutex_unlock(&queue_mutex);
    }
}

void free_and_close_connection(void* socket_fd){
    close(*((int*) socket_fd));
    free(socket_fd);
}
