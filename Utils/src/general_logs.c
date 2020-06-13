#include <commons/string.h>
#include <pretty_printer.h>
#include <stdlib.h>
#include <errno.h>
#include "../include/general_logs.h"
#include "logger.h"
#include "string.h"

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

void log_syscall_error(char* message_error){
    char* message = string_from_format("%s: %s", message_error, strerror(errno));
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_send_all_error(int sent_bytes, int amount_of_bytes){
    char* message = string_from_format("%s. Cantidad de bytes enviados: %d Total que debia enviarse: %d", "Error en send_all", sent_bytes, amount_of_bytes);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_expanding_unexpandable_matrix_error(){
    log_errorful_message(process_execution_logger(), "No se puede ampliar una matriz definida como no ampliable\n");
}

void log_invalid_index_error(){
    log_errorful_message(process_execution_logger(), "Los índices de filas y columnas deben ser mayores a cero\n");
}

void log_are_not_equals_columns_and_rows_in_squared_matrix_error(){
    log_errorful_message(process_execution_logger(), "Una matriz cuadrada debe tener igual cantidad de filas y columnas\n");
}

void log_invalid_positions_error(){
    log_errorful_message(process_execution_logger(), "Las posiciones elegidas exceden las dimensiones de la matriz\n");
}

void log_pthread_create_error(unsigned int process_id){
    char* message = string_from_format("Ocurrió un error al querer crear un hilo para el proceso: %u", process_id);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_queue_name_not_found_error(uint32_t queue_code){
    char* message = string_from_format("No se encontró una cola para el código: %lu", queue_code);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_queue_code_not_found_error(char* queue_name){
    char* message = string_new();
    string_append(&message, "No se encontró una cola para el nombre: ");
    string_append(&message, queue_name);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_printable_object_not_found_error(){
    log_errorful_message(process_execution_logger(), "No se encontró un printable_object válido.");
}

void log_request_with_event(t_log* logger, t_request* request, char* event){
    char* message = string_new();
    string_append(&message, event);
    string_append(&message, ":\n");

    char* request_string = request_pretty_print(request);
    string_append(&message, request_string);
    log_succesful_message(logger, message);
    free(request_string);
    free(message);
}

void log_request_received_with(t_log* logger, t_request* request){
    log_request_with_event(logger, request, "Pedido recibido");
}

void log_about_to_send_request(t_request* request){
    log_request_with_event(process_execution_logger(), request, "Pedido de enviar");
}

void log_request_sent(t_request* request){
    log_request_with_event(process_execution_logger(), request, "Pedido enviado");
}

void log_unknown_chained_evaluation_type_error(){
    log_errorful_message(process_execution_logger(), "Se ha configurado una chained_evaluation con un tipo inválido.");
}

void log_garbage_to_stop_considering_that_not_found_error(){
    log_errorful_message(process_execution_logger(), "Se esperaba encontrar un objeto designado como basura.");
}