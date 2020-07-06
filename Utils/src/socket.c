#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <zconf.h>
#include <stdint.h>
#include "../include/socket.h"
#include <commons/collections/queue.h>
#include <commons/process.h>
#include <pthread_wrapper.h>
#include <semaphore.h>
#include <garbage_collector.h>
#include <general_logs.h>

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
        log_syscall_error("Error al obtener ifaddrs");
        free_system();
    }

    struct ifaddrs* interface_adress_to_iterate = interface_addresses;

    while(interface_adress_to_iterate != NULL) {

        if (found_local_ip_address_in(interface_adress_to_iterate)) {
            af_inet_address_interface = (struct sockaddr_in*) interface_adress_to_iterate -> ifa_addr;
            local_ip_address = inet_ntoa(af_inet_address_interface -> sin_addr);
        }

        interface_adress_to_iterate = interface_adress_to_iterate -> ifa_next;
    }

    freeifaddrs(interface_addresses);

    if (local_ip_address == NULL) {
        log_syscall_error("Error al obtener ip local");
        free_system();
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
        free_system();
    }

    return address_interface;
}

int get_socket_fd_using(struct addrinfo* address_interface){
    int socket_fd;

    if ((socket_fd = socket(address_interface -> ai_family,
                            address_interface -> ai_socktype,
                            address_interface -> ai_protocol)) == -1) {
        log_syscall_error("Error al obtener socket_fd");
        freeaddrinfo(address_interface);
        free_system();
    }

    return socket_fd;
}

void allow_port_reusability(int socket_fd, struct addrinfo* address_interface){
    bool reuse_ports = true;

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_ports, sizeof(int)) == -1) {
        close(socket_fd);
        log_syscall_error("Error al habilitar reutilización de puerto");
        freeaddrinfo(address_interface);
        free_system();
    }
}

void bind_port_to_socket(int socket_fd, struct addrinfo* address_interface){

    if (bind(socket_fd, address_interface -> ai_addr, address_interface -> ai_addrlen) == -1) {
        close(socket_fd);
        log_syscall_error("Error de server al hacer bind");
        freeaddrinfo(address_interface);
        free_system();
    }
}

void listen_with(int socket_fd){

    if (listen(socket_fd, SOMAXCONN) == -1) {
        close(socket_fd);
        log_syscall_error("Error al escuchar con socket");
        free_system();
    }
}

int accept_incoming_connections_on(int socket_fd){
    int connection_fd;
    struct sockaddr_storage client_address;
    socklen_t address_size;

    address_size = sizeof client_address;
    connection_fd = accept(socket_fd, (struct sockaddr *) &client_address, &address_size);
    if (connection_fd == -1) {
        log_syscall_error("Error al aceptar conexiones en socket");
        free_system();
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
    log_syscall_error("Error de conexión");
    free_and_close_connection_information(connection_information);
    free_system();
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

    t_connection_information* connection_information = safe_malloc(sizeof(t_connection_information));
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
        partially_sent_bytes = send(socket_fd, serialized_request + sent_bytes, left_bytes, MSG_NOSIGNAL);

        if(partially_sent_bytes == -1){
            log_send_all_error(sent_bytes, amount_of_bytes);
            close(socket_fd);
            free_system();
        }
        sent_bytes += partially_sent_bytes;
        left_bytes -= partially_sent_bytes;
    }
}

void send_serialized_structure(t_serialization_information* serialization_information, int socket_fd) {

    uint32_t total_amount_of_bytes =
            serialization_information -> amount_of_bytes    // amount_of_bytes_of_request
            + sizeof(uint32_t);                             // total_amount

    void* serialized_request = safe_malloc(total_amount_of_bytes);

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
    send_serialized_structure(request_serialization_information, socket_fd);
    free_serialization_information(request_serialization_information);
}

void send_ack_message(uint32_t message_id, int socket_fd){

    void* serialized_ack = safe_malloc(sizeof(uint32_t));
    memcpy(serialized_ack, &message_id, sizeof(uint32_t));
    send_all(socket_fd, serialized_ack, sizeof(uint32_t));
    free(serialized_ack);
}

int serialize_and_send_structure_and_wait_for_ack(t_request* request, int socket_fd, int timeout_in_seconds){
    serialize_and_send_structure(request, socket_fd);

    void* ack = receive_ack_with_timeout_in_seconds(socket_fd, timeout_in_seconds);
    int cast_ack = *((int*) ack);
    free(ack);

    if(cast_ack == FAILED_ACK){
        log_failed_ack_error();
        free_system();
    }

    return cast_ack;
}

void set_receive_timeout_in_seconds(int socket_fd, int timeout_in_seconds){
    struct timeval timeval;
    timeval.tv_sec = timeout_in_seconds;
    timeval.tv_usec = 0;
    setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval*) &timeval, sizeof timeval);
}

void reset_receive_timeout_for(int socket_fd){
    set_receive_timeout_in_seconds(socket_fd, 0);
}

void* receive_ack_with_timeout_in_seconds(int socket_fd, int timeout_in_seconds){

    set_receive_timeout_in_seconds(socket_fd, timeout_in_seconds);
    uint32_t* ack = safe_malloc(sizeof(uint32_t));

    if(recv(socket_fd, ack, sizeof(uint32_t), MSG_WAITALL) == -1) {
        log_syscall_error("Error al recibir mensaje ACK");
        close(socket_fd);
        *ack = FAILED_ACK;
    }

    reset_receive_timeout_for(socket_fd);
    return (void*) ack;
}

void receive_failed(t_receive_information* receive_information){
    receive_information -> receive_was_successful = false;
    receive_information -> serialization_information = NULL;
}

t_serialization_information* create_serialization_information(uint32_t amount_of_bytes_of_request, void* serialized_request, t_serialization_information* serialization_information){
    serialization_information -> amount_of_bytes = amount_of_bytes_of_request;
    serialization_information -> serialized_request = serialized_request;

    return serialization_information;
}

t_receive_information* receive_structure(int socket_fd){

    void* serialized_request = NULL;
    uint32_t amount_of_bytes_of_request;
    t_serialization_information* serialization_information = safe_malloc(sizeof(t_serialization_information));
    t_receive_information* receive_information = safe_malloc(sizeof(t_receive_information));
    receive_information -> receive_was_successful = true;
    receive_information -> serialization_information = serialization_information;

    if(recv(socket_fd, &amount_of_bytes_of_request, sizeof(uint32_t), MSG_WAITALL) <= 0){
            log_syscall_error("Error al recibir estructura");
            close(socket_fd);
            receive_failed(receive_information);
        }

    if(receive_information -> receive_was_successful){
        serialized_request = safe_malloc(amount_of_bytes_of_request);

        if(recv(socket_fd, serialized_request, amount_of_bytes_of_request, MSG_WAITALL) <= 0){
            log_syscall_error("Error al recibir serialized_request");
            close(socket_fd);
            receive_failed(receive_information);
        }
    }

    if(receive_information -> receive_was_successful){
        serialization_information = create_serialization_information(amount_of_bytes_of_request, serialized_request, serialization_information);
        receive_information -> serialization_information = serialization_information;
    }

    return receive_information;

}

void start_multithreaded_server(char* port, void* (*handle_connection_function) (void*)){
    queue = queue_create();
    sem_initialize(&client_sockets_amount_in_queue);

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
            log_syscall_error("Error al crear hilos que atienden clientes");
            free_system();
        }
    }

    int server_socket_fd = listen_at(port);

    while(true){
        int* client_socket_fd = safe_malloc(sizeof(int));
        consider_as_garbage(client_socket_fd, free);

        *client_socket_fd = accept_incoming_connections_on(server_socket_fd);
        stop_considering_garbage(client_socket_fd);

        pthread_mutex_lock(&queue_mutex);
        queue_push(queue, (void*) client_socket_fd);
        sem_post(&client_sockets_amount_in_queue);
        pthread_mutex_unlock(&queue_mutex);
    }
}

void close_connection(int socket_fd){
    close(socket_fd);
}

void free_and_close_connection(void* socket_fd){
    close_connection(*((int*) socket_fd));
    free(socket_fd);
}

void free_multithreaded_server(){
    queue_destroy_and_destroy_elements(queue, free);

    for(int i = 0; i < THREAD_POOL_SIZE; i++){
        pthread_t thread = thread_pool[i];
        safe_thread_cancel(thread);
    }
}

void free_receive_information(t_receive_information* receive_information){

    if(receive_information ->receive_was_successful){
        free_serialization_information(receive_information -> serialization_information);
    }
    free(receive_information);
}