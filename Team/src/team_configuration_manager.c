#include "team_configuration_manager.h"
#include "../../Utils/include/configuration_manager.h"

char* ip;
char* port;

void initialize_team_configuration_manager(){
    initialize_configuration_manager_named("team-RR");

    ip = config_get_string_at("IP_BROKER");
    port = config_get_string_at("PUERTO_BROKER");
}

char* broker_ip(){
    return ip;
}

char* broker_port(){
    return port;
}