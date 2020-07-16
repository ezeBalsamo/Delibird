#include "../include/logger.h"
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <stdlib.h>
#include <paths.h>
#include <sys/stat.h>
#include <garbage_collector.h>
#include <general_logs.h>
#include <configuration_manager.h>

t_dictionary* loggers_by_name;
char* process_execution_log_name = "process-execution.log";
char* main_log_name;
char* program_name;

char* get_program_name(){
    return program_name;
}

void create_directory_if_necessary(){
    char* path = logs_path();

    if(!exists_file_at(path)){
        mkdir(path, 0700);
    }

    free(path);
}
t_log* existing_log_found_for(char* log_name){
    t_log* logger_found = (t_log*) dictionary_get(loggers_by_name,log_name);

    if (logger_found == NULL){
        log_errorful_not_existing_log(log_name);
        free_system();
    }
    return logger_found;
}

t_log* logger_named(char* log_name){
    return existing_log_found_for(log_name);
}

void create_log_named(char* log_name){
    char* log_absolute_path = absolute_path_for_log_named(log_name);
    t_log* logger = log_create(log_absolute_path, program_name, true, LOG_LEVEL_DEBUG);
    dictionary_put(loggers_by_name, log_name, (void*) logger);

    free(log_absolute_path);
}

void initialize_logger_for(char* program_name_parameter){
    loggers_by_name = dictionary_create();
    create_directory_if_necessary();
    program_name = program_name_parameter;
}

void create_main_logger_from_config(){
    char* logs_path = config_get_string_at("LOG_FILE");
    main_log_name = get_file_name_from_path(logs_path);
    create_log_named(main_log_name);
}

t_log* main_logger(){
    return dictionary_get(loggers_by_name, main_log_name);
}

void create_process_execution_logger(){
    create_log_named(process_execution_log_name);
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

void log_warning_message(t_log* logger, char* message){
    log_warning(logger, message);
}

void free_loggers(){

    if(main_log_name != NULL){
        free(main_log_name);
    }

    dictionary_destroy_and_destroy_elements(loggers_by_name, (void (*)(void *)) log_destroy);
}