#include "gamecard_configuration_manager.h"
#include "../../Utils/include/configuration_manager.h"

char* ip_broker;
char* port_broker;
int timeout_in_seconds;

void initialize_gamecard_configuration_manager(){
    initialize_configuration_manager_named("gamecard");

    ip_broker = config_get_string_at("IP_BROKER");
    port_broker = config_get_string_at("PUERTO_BROKER");
    timeout_in_seconds = config_get_int_at("ACK_TIMEOUT");
}

char* broker_ip(){
    return ip_broker;
}

char* broker_port(){
    return port_broker;
}

int ack_timeout(){
    return timeout_in_seconds;
}