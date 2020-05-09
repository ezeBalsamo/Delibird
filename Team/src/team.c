#include "../include/team_manager.h"
#include "../include/map.h"
#include "../include/broker_connection_handler.h"
#include "../include/gameboy_connection_handler.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/processes_information.h"

void initialize_team(){
    initialize_processes_information();
    initialize_configuration_manager_named("team-RR");
    initialize_team_manager();
    initialize_map();
    initialize_broker_connection_handler();
    initialize_gameboy_connection_handler();
}

int main(void){
    initialize_team();
}