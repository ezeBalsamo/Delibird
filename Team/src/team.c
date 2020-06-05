#include <dispatcher.h>
#include <team_configuration_manager.h>
#include "../include/team_logs_manager.h"
#include "../include/team_serializable_objects.h"
#include "../include/broker_connection_handler.h"
#include "../include/gameboy_connection_handler.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/free_system.h"

int main(void) {

    initialize_team_logs_manager();
    initialize_pretty_printer();
    initialize_team_serializable_objects();
    initialize_team_configuration_manager();

    initialize_dispatcher();

    log_succesful_start_up();

    pthread_t team_manager_thread = default_safe_thread_create(initialize_team_manager, NULL);
    pthread_t broker_connection_handler_thread = default_safe_thread_create(initialize_broker_connection_handler, NULL);
    pthread_t gameboy_connection_handler_thread = default_safe_thread_create(initialize_gameboy_connection_handler,NULL);

    thread_join(team_manager_thread);
    thread_join(broker_connection_handler_thread);
    thread_join(gameboy_connection_handler_thread);

    log_successful_execution();

    free_system();
}