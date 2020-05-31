#ifndef DELIBIRD_LOGGER_H
#define DELIBIRD_LOGGER_H

#include <commons/log.h>

void initialize_logs_manager_for(char* program_name);
void initialize_logger();
void create_main_logger_for(char* program_name);
t_log* main_logger();
void create_process_execution_logger_for(char* program_name);
t_log* process_execution_logger();
void create_delibird_logger_for(char* program_name);
t_log* delibird_logger();
void log_succesful_start_up();
void log_successful_execution();
void log_successful_clean_up();
void log_errorful_message(t_log* logger, char* message);
void log_succesful_message(t_log* logger, char* message);

void free_loggers();

#endif //DELIBIRD_LOGGER_H
