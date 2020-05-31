#include "../include/team_logs_manager.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include <commons/string.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

void initialize_team_logs_manager(){
    initialize_logger();
    create_main_logger_for("Team");
    create_process_execution_logger_for("Team");
}

void log_succesful_start_up(){
    log_succesful_message(process_execution_logger(), "Team se ha inicializado correctamente!\n");
}

void log_failed_attempt_to_communicate_with_broker(){
    char* message = "Falló la comunicación con Broker. Se procederá a reintentar.";
    log_errorful_message(main_logger(), message);
    log_errorful_message(process_execution_logger(), message);
}

void log_initiating_communication_retry_process_with_broker(){
    char* message = "Inicio del proceso de reintento de comunicación con el Broker.";
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
}

void log_succesful_retry_of_communication_with_broker(){
    char* message = "El proceso de reintento de comunicación con el Broker ha sido exitoso.";
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
}

void log_failed_retry_of_communication_with_broker(){
    char* message = "Falló el reintento de comunicación con el Broker.";
    log_errorful_message(main_logger(), message);
    log_errorful_message(process_execution_logger(), message);
}

void log_thread_sleep_time_configuration_error(){
    char* message = string_new();
    string_append(&message, "Se produjo un error al intentar dormir el hilo: ");
    string_append(&message, strerror(errno));

    log_errorful_message(main_logger(), message);
    log_errorful_message(process_execution_logger(), message);

    free(message);
    free_team_logs_manager();
    exit(EXIT_FAILURE);
}

void log_queue_thread_create_error(){
    char* message = string_new();
    string_append(&message, "Falló la creación del hilo para escuchar una cola del Broker: ");
    string_append(&message, strerror(errno));

    log_errorful_message(process_execution_logger(), message);

    free(message);
    free_team_logs_manager();
    exit(EXIT_FAILURE);
}
void log_invalid_operation_to_query_performer(uint32_t code){
    char* operation_name = queue_name_of(code);
    char* message;
    if (operation_name == NULL){
        message = string_from_format("Operacion invalida para que el query performer realice, codigo %d.",code);
    }else{
        message = string_from_format("Operacion %s invalida para que el query performer realice.", operation_name);
        free(operation_name);
    }

    log_errorful_message(process_execution_logger(),message);
    free(message);
}
void log_no_locations_found_for(char* pokemon_name){
    char* message = string_from_format("No se encontraron ubicaciones para %s\n", pokemon_name);

    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void free_team_logs_manager(){
    free_loggers();
}

void log_trainer_thread_create_error(){
    char* message = "Falló la creación de un hilo para un entrenador.";
    log_errorful_message(process_execution_logger(), message);

    free_team_logs_manager();
    exit(EXIT_FAILURE);
}