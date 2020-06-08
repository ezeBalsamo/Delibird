#include <team_serializable_objects.h>
#include <team_logs_manager.h>
#include <stdlib.h>
#include <map.h>
#include <dispatcher.h>
#include <gameboy_connection_handler.h>
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/free_system.h"

void free_system(){
    free_pretty_printer();
    free_team_serializable_objects();
    free_configuration_manager();
    free_dispatcher();
    free_trainer_threads();
    free_map();
    free_team_manager();
    free_gameboy_connection_handler();

    log_successful_clean_up();
    free_team_logs_manager();

    exit(EXIT_SUCCESS);
}