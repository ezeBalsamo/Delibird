#include "../include/team_manager.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/processes_information.h"

char* port(){
    return config_get_string_at("PUERTO");
}

void initialize_team(){
    initialize_processes_information();
    initialize_configuration_manager_named("team-RR");
    initialize_team_manager();
    multithreaded_server_listening_at(port());
    get_team_global_objective();
}

int main(void){
    initialize_team();
}