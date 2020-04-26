#include "../../Utils/include/socket.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/processes_information.h"

char* port(){
    return config_get_string_at("PUERTO");
}

void initialize_team(){
    initialize_processes_information();
    initialize_configuration_manager_named("team-RR");
    multithreaded_server_listening_at(port());
}

int main(void){
    initialize_team();
}