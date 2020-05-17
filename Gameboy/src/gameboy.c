#include "../include/entry_point_connection_builder.h"
#include "../include/entry_point_validator.h"
#include "../include/entry_point_logs_manager.h"
#include "../include/entry_point_executor.h"

int main(int arguments_amount, char* arguments[]) {

    initialize_entry_point_logs_manager();
    initialize_entry_point_validator(arguments_amount, arguments);
    initialize_entry_point_connection_builder();
    log_succesful_start_up();

    execute();

    free_entry_point_validator();
    free_entry_point_logs_manager();
    free_entry_point_connection_builder();

    return 0;
}
