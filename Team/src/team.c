#include "../include/team_manager.h"
#include "../include/map.h"
#include "../include/team_logs_manager.h"
#include "../include/broker_connection_handler.h"
#include "../include/gameboy_connection_handler.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/processes_information.h"
#include "../../Utils/include/pthread_wrapper.h"

int main(void){
    initialize_team_logs_manager();
    initialize_processes_information();
    //TODO: entry point log manager
    //initialize_entry_point_logs_manager();
    initialize_configuration_manager_named("team-RR");
    initialize_team_manager();
    t_list* team_global_objective = get_team_actual_global_objective();

    initialize_map();
    log_succesful_start_up();

    pthread_t broker_connection_handler_thread = thread_create(initialize_broker_connection_handler, NULL, default_thread_create_error_response_strategy);
    pthread_t gameboy_connection_handler_thread = thread_create(initialize_gameboy_connection_handler, NULL, default_thread_create_error_response_strategy);

    thread_join(broker_connection_handler_thread);
    thread_join(gameboy_connection_handler_thread);
}