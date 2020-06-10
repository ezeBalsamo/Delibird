#include <entry_point_validator.h>
#include <entry_point_connection_builder.h>
#include <entry_point_logs_manager.h>
#include <stdlib.h>
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/general_logs.h"
#include "free_gameboy.h"

void free_system(){
    free_entry_point_validator();
    free_entry_point_connection_builder();
    free_pretty_printer();

    free_garbage_collector();
    log_successful_clean_up();
    free_entry_point_logs_manager();

    exit(EXIT_SUCCESS);
}