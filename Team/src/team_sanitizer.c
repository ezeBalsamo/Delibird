#include <team_serializable_objects.h>
#include <team_logs_manager.h>
#include <stdlib.h>
#include <pokemon_occurrences.h>
#include <dispatcher.h>
#include <team_gameboy_connection_handler.h>
#include <team_broker_connection_handler.h>
#include <team_query_performers.h>
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/garbage_collector.h"

void free_system(){
    free_pretty_printer();
    free_team_serializable_objects();
    free_team_query_performers();
    free_dispatcher();
    free_trainer_threads();
    free_pokemon_occurrences();
    free_team_manager();
    free_team_gameboy_connection_handler();
    free_team_broker_connection_handler();

    free_garbage_collector();
    free_configuration_manager();
    log_successful_clean_up();
    free_team_logs_manager();

    exit(EXIT_SUCCESS);
}