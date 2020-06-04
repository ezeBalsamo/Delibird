#include "../include/team_logs_manager.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include <commons/string.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "../../Utils/include/free_system.h"

void initialize_team_logs_manager(){
    initialize_logger_for("Team");
    create_main_logger();
    create_process_execution_logger();
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
}

void log_queue_thread_create_error(){
    char* message = string_new();
    string_append(&message, "Falló la creación del hilo para escuchar una cola del Broker: ");
    string_append(&message, strerror(errno));

    log_errorful_message(process_execution_logger(), message);

    free(message);
    free_system();
}

void log_no_locations_found_for(char* pokemon_name){
    char* message = string_from_format("No se encontraron ubicaciones para %s\n", pokemon_name);

    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_trainer_thread_create_error(){
    log_errorful_message(process_execution_logger(), "Falló la creación de un hilo para un entrenador.");
    free_system();
}

void log_succesful_creation_of_thread_of_trainer(uint32_t sequential_number){
    char* message = string_from_format("Se creó exitosamente el hilo para el entrenador %u", sequential_number);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_query_performer_not_found_error_for(uint32_t operation){
    char* message = string_from_format("No se ha encontrado un query performer que maneje operaciones de código %u", operation);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_zero_schedulable_threads_error(){
    log_errorful_message(process_execution_logger(), "Se esperaba encontrar al menos un hilo planificable.");
}

void log_synchronizable_trainer_not_found_error_for(uint32_t sequential_number){
    char* message = string_from_format("Se esperaba encontrar al entrenador %u en las colas de new o blocked.", sequential_number);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_scheduling_algorithm_not_found_error_for(char* scheduling_algorithm_name){
    char* message = string_from_format("No se ha implementado un algoritmo de planificación llamado %s", scheduling_algorithm_name);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void free_team_logs_manager(){
    free_loggers();
}