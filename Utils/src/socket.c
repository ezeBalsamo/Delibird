#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <pthread.h>
#include "../include/socket.h"
#include <stdint.h>

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
    int reuse_ports = 1;

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

int reconnect(t_connection_information* connection_information){
    return connect(connection_information -> socket_fd,
            connection_information -> address_interface -> ai_addr,
            connection_information -> address_interface -> ai_addrlen);
}

void close_connection_strategy(t_connection_information* connection_information){
    close(connection_information -> socket_fd);
    perror("connect error");
    freeaddrinfo(connection_information -> address_interface);
    free(connection_information);
    exit(EXIT_FAILURE);
}

void establish_connection(t_connection_information* connection_information, void (*disconnection_strategy) (t_connection_information*)){

    if (connect(connection_information -> socket_fd,
                connection_information -> address_interface -> ai_addr,
                connection_information -> address_interface -> ai_addrlen) == -1) {

        (*disconnection_strategy) (connection_information);
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

    printf("Incoming connection accepted with socket file descriptor %d\n", connection_fd);

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

int connect_to(char* ip, char* port, void (*disconnection_strategy) (t_connection_information*)) {

    struct addrinfo* address_interface = build_address_interface(ip, port);
    int socket_fd = get_socket_fd_using(address_interface);

    t_connection_information* connection_information = malloc(sizeof(t_connection_information*));
    connection_information -> socket_fd = socket_fd;
    connection_information -> address_interface = address_interface;

    establish_connection(connection_information, disconnection_strategy);

    freeaddrinfo(address_interface);
    free(connection_information);
    return socket_fd;
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

void send_structure(t_request* request, int socket_fd){

    t_serialization_information* serialization_information = serialize(request);
    send_all(socket_fd, serialization_information -> serialized_request, serialization_information -> amount_of_bytes);
    free_serialization_information(serialization_information);
}

void* receive_structure(int socket_fd){

    void* serialized_request;
    uint32_t amount_of_bytes;

    if(recv(socket_fd, &amount_of_bytes, sizeof(uint32_t), MSG_WAITALL) == -1){
        perror("recv amount of bytes error");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    serialized_request = malloc(amount_of_bytes);

    if(recv(socket_fd, serialized_request, amount_of_bytes, MSG_WAITALL) == -1){
        perror("recv serialized structure error");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    return serialized_request;
}

void multithreaded_server_listening_at(char* port){
    int server_socket_fd = listen_at(port);

    while(1){
        int connection_fd = accept_incoming_connections_on(server_socket_fd);
        void* serialized_request = receive_structure(connection_fd);
        pthread_t tid;

        if(pthread_create(&tid, NULL, deserialize, serialized_request) != 0){
            printf("An error occurred while creating a new thread for attending an incoming connection\n");
            close(server_socket_fd);
            close(connection_fd);
            exit(EXIT_FAILURE);
        }

        pthread_join(tid, NULL);
        close(connection_fd);
    }
}