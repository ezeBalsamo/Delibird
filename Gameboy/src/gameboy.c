#include <stdlib.h>
#include "../include/entry_point_connection_builder.h"
#include "../include/entry_point_validator.h"
#include "../include/entry_point_logs_manager.h"
#include "../include/entry_point_executor.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/general_logs.h"
#include "free_gameboy.h"

int main(int arguments_amount, char* arguments[]) {

    initialize_entry_point_logs_manager();
    initialize_entry_point_connection_builder();
    initialize_pretty_printer();
    initialize_entry_point_validator(arguments_amount, arguments);

    log_succesful_start_up();

    execute();

    log_successful_execution();

    free_system();

    return EXIT_SUCCESS;
}
