#include <commons/string.h>
#include <stdlib.h>
#include "../include/entry_point_logs_manager.h"
#include "../include/entry_point_validator.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/pretty_printer.h"

void initialize_entry_point_logs_manager(){
    initialize_logger_for("Gameboy");
    create_main_logger();
    create_process_execution_logger();
}

void log_successful_connection(){
    char* message = string_new();
    string_append(&message, "Conexión establecida al proceso ");
    string_append(&message, valid_chosen_process() -> name);
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_succesful_initialize_entry_point_validator(){
    log_succesful_message(process_execution_logger(), "Entry point validator se ha inicializado correctamente!\n");
}

void unknown_process_error_for(char* process_name){
    char* message = string_new();
    string_append(&message, process_name);
    string_append(&message, " no está registrado como un proceso válido.\n");

    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void unknown_operation_error_for(char* process_name, char* operation_name){
    char* message = string_new();
    string_append(&message, operation_name);
    string_append(&message, " no está registrada como una operación válida para ");
    string_append(&message, process_name);
    string_append(&message, ".\n");

    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void incorrect_arguments_amount_error(){
    log_errorful_message(process_execution_logger(), "La cantidad de argumentos ingresada es incorrecta.\n");
}

void log_no_parser_suitable_for_operation_error_for(char* operation_name){
    char* message = string_from_format("No se ha encontrado un parser compatible para la operación %s.", operation_name);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void free_entry_point_logs_manager(){
    free_loggers();
}
