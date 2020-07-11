#include "team_configuration_manager.h"
#include "../../Utils/include/configuration_manager.h"

char* ip;
char* port;
int time_delay_in_seconds;
int timeout_in_seconds;

void initialize_team_configuration_manager(){
    initialize_configuration_manager();

    ip = config_get_string_at("IP_BROKER");
    port = config_get_string_at("PUERTO_BROKER");
    time_delay_in_seconds = config_get_int_at("RETARDO_CICLO_CPU");
    timeout_in_seconds = config_get_int_at("ACK_TIMEOUT");
}

char* broker_ip(){
    return ip;
}

char* broker_port(){
    return port;
}

int time_delay(){
    return time_delay_in_seconds;
}

int ack_timeout(){
    return timeout_in_seconds;
}