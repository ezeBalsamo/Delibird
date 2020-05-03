#include <commons/string.h>
#include <stdlib.h>
#include "../include/entry_point_logs_manager.h"
#include "../include/entry_point_validator.h"
#include "../../Utils/include/logger.h"

void free_entry_point_logs_manager(){
    free_loggers();
}

char* request_as_string(t_request* request){
    char* message = string_new();
    string_append(&message, "Operación: ");
    t_operation_information* operation_information = valid_chosen_operation();
    string_append(&message, operation_information -> name);
    string_append(&message, "\n");
    string_append(&message,"Argumentos: ");
    char ** arguments = (char **) request -> structure;

    for(int i = 0; arguments[i]; i++){
        string_append(&message, arguments[i]);
        string_append(&message, " ");
    }
    string_append(&message, "\n");
    return message;
}

void log_successful_connection(){
    char* message = string_new();
    string_append(&message, "Conexión establecida al proceso ");
    string_append(&message, valid_chosen_process() -> name);
    log_succesful_message(main_logger(), message);
    free(message);
}

void log_request_with_event(t_request* request, char* event){
    char* message = string_new();
    string_append(&message, event);
    string_append(&message, ":\n");

    char* request_string = request_as_string(request);
    string_append(&message, request_string);
    log_succesful_message(process_execution_logger(), message);
    free(request_string);
    free(message);
}

void log_about_to_send_request(t_request* request){
    log_request_with_event(request, "Pedido de enviar");
}

void log_request_sent(t_request* request){
    log_request_with_event(request, "Pedido enviado");
}

void unknown_process_error_for(char* process_name){
    char* message = string_new();
    string_append(&message, process_name);
    string_append(&message, " no está registrado como un proceso válido.\n");

    log_errorful_message(process_execution_logger(), message);
    free(message);
    free_entry_point_logs_manager();
    exit(EXIT_FAILURE);
}

void unknown_operation_error_for(char* process_name, char* operation_name){
    char* message = string_new();
    string_append(&message, operation_name);
    string_append(&message, " no está registrada como una operación válida para ");
    string_append(&message, process_name);
    string_append(&message, ".\n");

    log_errorful_message(process_execution_logger(), message);
    free(message);
    free_entry_point_logs_manager();
    exit(EXIT_FAILURE);
}

void incorrect_arguments_amount_error(){
    log_errorful_message(process_execution_logger(), "La cantidad de argumentos ingresada es incorrecta.\n");
    free_entry_point_logs_manager();
    exit(EXIT_FAILURE);
}

void initialize_entry_point_logs_manager(){
    initialize_logger();
    create_main_logger_for("Gameboy");
    create_process_execution_logger_for("Gameboy");
}

void log_succesful_start_up(){
    log_succesful_message(process_execution_logger(), "Gameboy has succesfully started!\n");
}