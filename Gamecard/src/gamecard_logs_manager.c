#include "../include/gamecard_logs_manager.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include <commons/string.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

void initialize_gamecard_logs_manager(){
    initialize_logger_for("Gamecard");
    create_main_logger_from_config();
    create_process_execution_logger();
}

void log_initiating_communication_retry_process_with_broker_from_gamecard(){
    char* message = "Inicio del proceso de reintento de comunicación con el Broker.";
    log_succesful_message(process_execution_logger(), message);
}

void log_succesful_reception_of_message(t_identified_message* identified_message){
    char* message = string_from_format("Se recibio el mensaje con operación: %s con id = %d\n", queue_name_of(internal_operation_in(identified_message)) ,identified_message -> message_id);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_succesful_retry_of_communication_with_broker_from_gamecard(){
    char* message = "El proceso de reintento de comunicación con el Broker ha sido exitoso.";
    log_succesful_message(process_execution_logger(), message);
}

void log_failed_attempt_to_communicate_with_broker_from_gamecard(char* default_action){
    char* message = string_from_format("Falló la comunicación con Broker. Por defecto, %s.", default_action);
    log_errorful_message(main_logger(), message);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_pokemon_not_found_error(t_catch_pokemon* pokemon_to_subtract){
    char* message = string_from_format("No se encontró un pokemon con nombre: %s en la posicion: Pos x: %u Pos y: %u", pokemon_to_subtract -> pokemon_name, pokemon_to_subtract -> pos_x, pokemon_to_subtract -> pos_y);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}


void log_failed_retry_of_communication_with_broker_from_gamecard(){
    char* message = "Falló el reintento de comunicación con el Broker.";
    log_errorful_message(process_execution_logger(), message);
}

void log_thread_sleep_time_configuration_error_from_gamecard(){
    char* message = string_new();
    string_append(&message, "Se produjo un error al intentar dormir el hilo: ");
    string_append(&message, strerror(errno));

    log_errorful_message(process_execution_logger(), message);

    free(message);
    free_gamecard_logs_manager();
    exit(EXIT_FAILURE);
}

void log_queue_thread_create_error_from_gamecard(){
    char* message = string_new();
    string_append(&message, "Falló la creación del hilo para escuchar una cola del Broker: ");
    string_append(&message, strerror(errno));

    log_errorful_message(process_execution_logger(), message);

    free(message);
    free_gamecard_logs_manager();
    exit(EXIT_FAILURE);
}

void log_query_performer_not_found_error_from_gamecard_for(uint32_t operation){
    char* message = string_from_format("No se ha encontrado un query performer que maneje operaciones de código %u\n", operation);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_file_system_metadata_info(t_file_system_metadata* pointer_file_system_metadata){
    char* message = string_from_format("Tamanio: %d, Cantidad de Bloques: %d, Magic Number: %s\n", pointer_file_system_metadata -> block_size, pointer_file_system_metadata -> blocks, pointer_file_system_metadata -> magic_number);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_file_metadata_info(t_file_metadata* pointer_file_metadata){
    char* message = string_from_format("La Metadata del file system tiene Directorio: %s, Tamanio: %d, Bloques: %s, Abierto: %s\n", pointer_file_metadata -> directory, pointer_file_metadata -> size,  pointer_file_metadata -> blocks, pointer_file_metadata -> open);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_block_metadata_info(int32_t x, int32_t y, int32_t quantity){
    char* message = string_from_format("Se leyo una linea con Pos X: %d, Pos Y: %d, Cantidad: %d\n", x, y, quantity);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_unknown_file_type_error(){
    char* message = string_from_format("Lectura de archivo de tipo no identificado\n");
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_pokemon_file_destroyed(char* pokemon_name){

    char* message = string_from_format("Se eliminó del file system el Pokemon: %s\n", pokemon_name);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void free_gamecard_logs_manager(){
    free_loggers();
}