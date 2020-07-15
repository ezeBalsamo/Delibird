#include <dispatcher.h>
#include <team_configuration_manager.h>
#include <main_threads_executor.h>
#include <event_notifier.h>
#include "../include/team_logs_manager.h"
#include "../include/team_serializable_objects.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/garbage_collector.h"

int main(void) {

    initialize_signal_handler();
    initialize_garbage_collector();
    initialize_team_configuration_manager();

    initialize_main_threads_executor();
    initialize_team_logs_manager();
    initialize_pretty_printer();
    initialize_team_serializable_objects();
    initialize_event_notifier();
    initialize_dispatcher();

    log_succesful_start_up();

    execute_main_threads();

    log_successful_execution();

    free_system();
}