#ifndef DELIBIRD_ENTRY_POINT_LOGS_MANAGER_H
#define DELIBIRD_ENTRY_POINT_LOGS_MANAGER_H

#include "../../Utils/include/common_structures.h"

void log_successful_connection();
void log_succesful_initialize_entry_point_validator();
void log_succesful_initialize_entry_point_connection_builder();
void unknown_process_error_for(char* process_name);
void unknown_operation_error_for(char* process_name, char* operation_name);
void incorrect_arguments_amount_error();
void log_about_to_send_request(t_request* request);
void log_request_sent(t_request* request);
void log_no_parser_suitable_for_operation_error_for(char* operation_name);
void free_entry_point_logs_manager();

#endif //DELIBIRD_ENTRY_POINT_LOGS_MANAGER_H
