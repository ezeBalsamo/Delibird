#ifndef DELIBIRD_LOGGER_H
#define DELIBIRD_LOGGER_H

#include <commons/log.h>

void initialize_logger_for(char* program_name);
char* get_program_name();
void create_main_logger();
void create_log_named(char* log_name);
t_log* main_logger();
void create_process_execution_logger();
t_log* process_execution_logger();
t_log* cache_dump_logger();
void log_errorful_message(t_log* logger, char* message);
void log_succesful_message(t_log* logger, char* message);
void log_warning_message(t_log* logger, char* message);

void free_loggers();

#endif //DELIBIRD_LOGGER_H
