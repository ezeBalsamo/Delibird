#include <gamecard_logs_manager.h>
#include <gamecard_serializable_objects.h>
#include <gameboy_connection_handler.h>
#include <broker_connection_handler.h>
#include <stdlib.h>
#include <gamecard_query_performers.h>
#include <filesystem_files_information.h>
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/garbage_collector.h"

void free_system(){

    free_pretty_printer();
    free_gamecard_serializable_objects();
    free_gameboy_connection_handler();
    free_gamecard_broker_connection_handler();
    free_gamecard_query_performers();
    free_files_information();

    free_garbage_collector();
    free_configuration_manager();
    log_successful_clean_up();
    free_gamecard_logs_manager();

    exit(EXIT_SUCCESS);
}