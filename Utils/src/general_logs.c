#include <commons/string.h>
#include <pretty_printer.h>
#include <stdlib.h>
#include "../include/general_logs.h"
#include "logger.h"

void log_succesful_start_up(){
    char* message = string_new();
    string_append(&message, get_program_name());
    string_append(&message," se ha inicializado correctamente!\n");
    char* blue_message = change_message_colour(message, blue_colour());
    log_succesful_message(process_execution_logger(), blue_message);
    free(message);
    free(blue_message);
}

void log_successful_execution(){
    char *message = string_new();
    string_append(&message, get_program_name());
    string_append(&message, " ha ejecutado exitosamente!\n");
    char* blue_message = change_message_colour(message, blue_colour());
    log_succesful_message(process_execution_logger(), blue_message);
    free(message);
    free(blue_message);
}

void log_successful_clean_up(){
    char *message = string_new();
    string_append(&message, get_program_name());
    string_append(&message, " ha liberado toda la memoria ocupada exitosamente!\n");
    char* blue_message = change_message_colour(message, blue_colour());
    log_succesful_message(process_execution_logger(), blue_message);
    free(message);
    free(blue_message);
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
