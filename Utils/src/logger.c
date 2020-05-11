#include "../include/logger.h"
#include <commons/string.h>
#include <commons/collections/dictionary.h>

t_dictionary* loggers_by_name;

t_log* process_execution_logger(){
    return dictionary_get(loggers_by_name, "../logs/process-execution.log");
}

t_log* main_logger(){
    return dictionary_get(loggers_by_name, "../logs/main.log");
}

void log_succesful_message(t_log* logger, char* message){
    log_info(logger, message);
}

void log_errorful_message(t_log* logger, char* message){
    log_error(logger, message);
}

void create_log(char* log_name, char* program_name){
    t_log* logger = log_create(log_name, program_name, true, LOG_LEVEL_DEBUG);
    dictionary_put(loggers_by_name, log_name, (void*) logger);
}

void create_process_execution_logger_for(char* program_name){
    create_log("../logs/process-execution.log", program_name);
}

void create_main_logger_for(char* program_name){
    create_log("../logs/main.log", program_name);
}

void initialize_logger(){
    loggers_by_name = dictionary_create();
}

void free_loggers(){
    dictionary_destroy_and_destroy_elements(loggers_by_name, (void (*)(void *)) log_destroy);
}