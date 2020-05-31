#include <commons/string.h>
#include "../include/general_logs.h"
#include "logger.h"

char* program;

void initialize_general_logs(char* program_name){
    program = program_name;
}

void log_succesful_start_up(){
    char* message = string_new();
    string_append(&message,program);
    string_append(&message," se ha inicializado correctamente!\n");
    log_debug(process_execution_logger(), message); //debug para que se vea en amarillo.
}

void log_successful_execution(){
    char *message = string_new();
    string_append(&message, program);
    string_append(&message, " ha ejecutado exitosamente!\n");
    log_debug(process_execution_logger(), message);
}

void log_successful_clean_up(){
    char *message = string_new();
    string_append(&message, program);
    string_append(&message, " ha liberado toda la memoria ocupada exitosamente!\n");
    log_debug(process_execution_logger(), message);
}

void log_succesful_initialize_config_manager(){
    log_succesful_message(process_execution_logger(), "El configuration_manager se ha inicializado correctamente!\n");
}

void log_succesful_initialize_pretty_printer(){
    log_succesful_message(process_execution_logger(), "Pretty printer se ha inicializado correctamente!\n");
}

void log_succesful_initialize_serializable_objects(){
    log_succesful_message(process_execution_logger(), "Los serializable_objects se han inicializado correctamente!\n");
}
