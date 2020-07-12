#include <team_serializable_objects.h>
#include <team_logs_manager.h>
#include <stdlib.h>
#include <pokemon_occurrences.h>
#include <dispatcher.h>
#include <team_gameboy_connection_handler.h>
#include <team_broker_connection_handler.h>
#include <main_threads_executor.h>
#include <event_notifier.h>
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/garbage_collector.h"

void free_system(){

    free_main_threads_executor();
    free_pretty_printer();
    free_team_serializable_objects();
    free_event_notifier();
    free_dispatcher();
    free_team_manager();
    free_team_gameboy_connection_handler();
    free_team_broker_connection_handler();

    free_garbage_collector();
    free_configuration_manager();
    log_successful_clean_up();
    free_team_logs_manager();

    exit(EXIT_SUCCESS);
}