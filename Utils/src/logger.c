#include "../include/logger.h"
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <roots.h>
#include <general_logs.h>

t_dictionary* loggers_by_name;
char* process_execution_log_name = "process-execution.log";
char* main_log_name = "main.log";

bool exists_directory_at(char* path){
    struct stat stat_buffer;
    return stat(path, &stat_buffer) != -1;
}

void create_directory_if_necessary(){
    char* path = logs_path();

    if(!exists_directory_at(path)){
        mkdir(path, 0700);
    }

    free(path);
}

void create_log(char* log_name, char* program_name){
    char* log_absolute_path = absolute_path_for_log_named(log_name);
    t_log* logger = log_create(log_absolute_path, program_name, true, LOG_LEVEL_DEBUG);
    dictionary_put(loggers_by_name, log_name, (void*) logger);

    free(log_absolute_path);
}

void initialize_logger(){
    loggers_by_name = dictionary_create();
    create_directory_if_necessary();
}

void initialize_logs_manager_for(char* program_name){
    initialize_logger();
    create_main_logger_for(program_name);
    create_process_execution_logger_for(program_name);
    initialize_general_logs(program_name);
}

void create_main_logger_for(char* program_name){
    create_log(main_log_name, program_name);
}

t_log* main_logger(){
    return dictionary_get(loggers_by_name, main_log_name);
}

void create_process_execution_logger_for(char* program_name){
    create_log(process_execution_log_name, program_name);
}

t_log* process_execution_logger(){
    return dictionary_get(loggers_by_name, process_execution_log_name);
}

void log_succesful_message(t_log* logger, char* message){
    log_info(logger, message);
}

void log_errorful_message(t_log* logger, char* message){
    log_error(logger, message);
}

void free_loggers(){
    dictionary_destroy_and_destroy_elements(loggers_by_name, (void (*)(void *)) log_destroy);
}