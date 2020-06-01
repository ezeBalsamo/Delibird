#include <stdlib.h>
#include "../include/entry_point_connection_builder.h"
#include "../include/entry_point_validator.h"
#include "../include/entry_point_logs_manager.h"
#include "../include/entry_point_executor.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/configuration_manager.h"

int main(int arguments_amount, char* arguments[]) {

    initialize_gameboy_logs_manager();
    initialize_pretty_printer();
    initialize_entry_point_validator(arguments_amount, arguments);
    initialize_configuration_manager_named("gameboy");

    log_succesful_start_up();

    execute();

    log_successful_execution();

    free_entry_point_validator();
    free_entry_point_connection_builder();
    free_pretty_printer();
    log_successful_clean_up();
    free_entry_point_logs_manager();

    return EXIT_SUCCESS;
}
