#include <commons/string.h>
#include <stdlib.h>
#include <subscriber_context_provider.h>
#include <queue_message_status.h>
#include "../include/broker_logs_manager.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/queue_code_name_associations.h"

char* cache_dump_log_name = "cache_dump.log";

void initialize_broker_logs_manager(){
    initialize_logger_for("Broker");
    create_main_logger();
    create_log_named(cache_dump_log_name);
    create_process_execution_logger();
}

//LOGS MAIN, LOS QUE VAN SI O SI EN EL TP.
//---------------------------------------------------------------------------------------

void log_succesful_connection_of_a_process(){
    char* message = "Se conectó un proceso correctamente.";
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
}

void log_succesful_subscription_process(t_subscriber_context* subscriber_context){
    char* message = string_from_format("Se suscribió al proceso: %s con socket: %d a la cola de mensajes: %s correctamente!", subscriber_context -> process_description, subscriber_context -> socket_fd, queue_name_of(subscriber_context -> operation_queue));
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_succesful_new_message_pushed_to_a_queue(t_identified_message* identified_message, uint32_t queue_code){
    char* printed_object = request_pretty_print(identified_message -> request);
    char* message = string_from_format("Se pusheo el mensaje:\n%s con id:%d \na la cola de mensajes: %s correctamente!", printed_object, identified_message -> message_id, queue_name_of(queue_code));
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(printed_object);
    free(message);
}

void log_succesful_message_sent_to_a_suscriber(t_request* request, t_subscriber_context* subscriber_context){
    char* printed_object = request_pretty_print(request);
    char* message = string_from_format("Se envió correctamente el mensaje:\n%s\nal suscriptor: %s con socket: %d", printed_object, subscriber_context -> process_description, subscriber_context -> socket_fd);
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(printed_object);
    free(message);
}

void log_succesful_message_received_by(t_subscriber_context* subscriber_context, t_message_status* message_status){
    char* printed_object = request_pretty_print(message_status -> identified_message -> request);
    char* message = string_from_format("El suscriptor: %s recibió el mensaje: \n%s\n con id: %d\n",subscriber_context -> process_description,printed_object, message_status -> identified_message -> message_id);
    log_succesful_message(process_execution_logger(), message);
    free(printed_object);
    free(message);
}

void log_succesful_save_message_to_cache(t_request* request, void* message_position){
    char* printed_object = request_pretty_print(request);
    char* message = string_from_format("Se guardó correctamente el mensaje:\n%s\nen la posicion de memoria: %p", printed_object, message_position);
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(printed_object);
    free(message);
}

void log_succesful_free_partition_to_cache(void* message_position){
    char* message = string_from_format("Se liberó correctamente un mensaje en la posicion de memoria: %p", message_position);
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_succesful_memory_compaction(int amount_of_partitions_compacted) {
    char *message = string_from_format("Se compactó correctamente la memoria, compactando %d bloque/s.",
                                       amount_of_partitions_compacted);
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_cache_dump_information(char* cache_info){
    t_log* cache_dump_logger_found = logger_named(cache_dump_log_name);
    log_info(cache_dump_logger_found, cache_info);
    log_succesful_message(process_execution_logger(), "Se recibio el pedido de dump a la cache y fue realizado correctamente!");
}


//---------------------------------------------------------------------
                //LOGS PARA NOSOTROS, PARA CONTROL
//---------------------------------------------------------------------

void log_succesful_initialize_queue_context_provider(){
    log_succesful_message(process_execution_logger(), "El queue_context_provider se ha inicializado correctamente!\n");
}
void log_successful_initialize_broker_memory_manager(){
    log_succesful_message(process_execution_logger(), "El broker_memory_manager se ha inicializado correctamente!\n");
}
void log_server_initial_status(){
    log_succesful_message(process_execution_logger(), "El server multihilo fue levantado y esta esperando recibir información.\n");
}

void log_structure_received(void* serialized_request){
    char* printed_object = request_pretty_print(serialized_request);
    char* message = string_from_format("El server recibió el siguiente mensaje:\n %s", printed_object);
    log_succesful_message(process_execution_logger(), message);
    free(printed_object);
    free(message);
}

void log_succesful_message_sent_to_suscribers(t_request* request){
    char* printed_object = request_pretty_print(request);
    char* message = string_from_format("El siguiente mensaje fue enviado correctamente a todos los suscriptores:\n %s", printed_object);
    log_succesful_message(process_execution_logger(), message);
    free(printed_object);
    free(message);
}

void log_succesful_get_and_update_subscribers_to_send(t_identified_message* identified_message){
    char* printed_object = request_pretty_print(identified_message -> request);
    char* message = string_from_format("Se actualizaron los suscriptores a enviar del mensaje:\n%s con id: %d\n",printed_object, identified_message -> message_id);
    log_succesful_message(process_execution_logger(), message);
    free(printed_object);
    free(message);
}

void log_no_subscribers_for_request(t_request* request){
    char* printed_object = request_pretty_print(request);
    char* message = string_from_format("No hay suscriptores en la cola donde se encuentra este mensaje:\n%s", printed_object);
    log_succesful_message(process_execution_logger(), message);
    free(printed_object);
    free(message);
}

void log_update_of_message_id_received_for(t_subscriber_context* subscriber_context){
    char* message = string_from_format("Se le actualizo al suscriptor: %s su ultimo mensaje id recibido con el id: %zu", subscriber_context -> process_description ,  subscriber_context -> last_message_id_received);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_succesful_eliminating_message_of_a_queue(t_message_status* message_status, char* reason){
    char* printed_object = request_pretty_print(message_status ->identified_message -> request);
    char* message = string_from_format("Se borro el mensaje:\n %s\n con id: %d correctamente! Motivo de borrado: %s", printed_object, message_status -> identified_message -> message_id, reason);
    log_succesful_message(process_execution_logger(), message);
    free(printed_object);
    free(message);
}

void log_subscriber_disconnection(t_subscriber_context* subscriber_context){
    char* message = string_from_format("Se desconecto al suscriptor: %s con socket: %d de la cola: %s", subscriber_context -> process_description, subscriber_context -> socket_fd, queue_name_of(subscriber_context -> operation_queue));
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_invalid_operation_to_message_role_identifier_error(uint32_t operation){
    char* message = string_from_format("No se encontró un rol que maneje a la siguiente operación: %d.", operation);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_invalid_operation_to_save_message_error(){
    char* message = "No se pudo guardar el mensaje. Posiblemente sea mas grande que la memoria.";
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_invalid_free_partition_error(){
    char* message = "No se pudo encontrar particion victima para liberar. Posiblemente esten todas libres y no se debio llegar a este punto.";
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_invalid_minimum_partition_size_for_buddy_system_error(){
    char* message = "El tamaño de particion minima especificado es invalido para buddy system.";
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_invalid_memory_size_for_buddy_system_error(){
    char* message = "El tamaño de la memoria total especificado es invalido para buddy system.";
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_failed_to_receive_ack_error(t_subscriber_context* subscriber_context){
    char* message = string_from_format("Se esperaba recibir un ack del suscriptor %s en el socket %d.", subscriber_context -> process_description, subscriber_context -> socket_fd);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_subscriber_not_found_in_message_status_subscribers_error(t_subscriber_context* subscriber_context, t_identified_message* identified_message){

    char* printed_object = request_pretty_print(identified_message -> request);
    char* message = string_from_format("No se encontro al suscriptor: %s para removerlo de la lista de este mensaje:", subscriber_context -> process_description, printed_object);
    log_errorful_message(process_execution_logger(), message);
    free(printed_object);
    free(message);
}

void log_subscriber_not_found_in_queue_subscribers(t_subscriber_context* subscriber_context, uint32_t queue_code){

    char* process_description = subscriber_context -> process_description;
    char* queue_name = queue_name_of(queue_code);
    char* message =
            string_from_format("Se suscribio un suscriptor nuevo: %s a la cola de mensajes %s\n correctamente.",
                    process_description, queue_name);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_message_status_not_found_in_queue_error(uint32_t message_id){
    char* message = string_from_format("No se pudo borrar el mensaje id:'%d' de la cola de mensajes, posiblemente ya haya sido borrado de la cola por haber sido enviado a todos sus subscriptore (revisar logs).",message_id);
    log_warning_message(process_execution_logger(), message);
    free(message);
}

void free_broker_logs_manager(){
    free_loggers();
}