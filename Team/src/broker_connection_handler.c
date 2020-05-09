#include "../include/broker_connection_handler.h"
#include "../include/team_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/socket.h"
#include <stdlib.h>

int appeared_queue_socket_fd;
int caught_queue_socket_fd;
int localized_queue_socket_fd;

char* ip;
char* port;

char* broker_ip(){
    return config_get_string_at("IP_BROKER");
}

char* broker_port(){
    return config_get_string_at("PUERTO_BROKER");
}

void subscribe_to_queues(){

    appeared_queue_socket_fd = connect_to(ip, port);
    caught_queue_socket_fd = connect_to(ip, port);
    localized_queue_socket_fd = connect_to(ip, port);
}

void send_get_pokemon_request_of(t_pokemon_goal* pokemon_goal){

    t_request* request = malloc(sizeof(t_request));
    request -> operation = GET_POKEMON;
    request -> structure = (void*) (pokemon_goal -> pokemon_name);

    int socket_fd = connect_to(ip, port);
    send_structure(request, socket_fd);
    free(request);
}

void initialize_broker_connection_handler(){
    ip = broker_ip();
    port = broker_port();

    subscribe_to_queues();
    with_global_goal_do(send_get_pokemon_request_of);
}