#include "../include/team_manager.h"
#include "../include/map.h"
#include "../include/team_logs_manager.h"
#include "../include/team_operations_information.h"
#include "../include/broker_connection_handler.h"
#include "../include/gameboy_connection_handler.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/pthread_wrapper.h"

int main(void){
    initialize_team_logs_manager();
    initialize_team_operations_information();
    initialize_configuration_manager_named("team-RR");
    initialize_team_manager();

    initialize_map();
    log_succesful_start_up();

    pthread_t broker_connection_handler_thread = default_safe_thread_create(initialize_broker_connection_handler, NULL);
    pthread_t gameboy_connection_handler_thread = default_safe_thread_create(initialize_gameboy_connection_handler, NULL);

    thread_join(broker_connection_handler_thread);
    thread_join(gameboy_connection_handler_thread);

    free_team_manager();
}