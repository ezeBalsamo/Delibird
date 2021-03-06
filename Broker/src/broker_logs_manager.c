#include <commons/string.h>
#include <stdlib.h>
#include <subscriber_context_provider.h>
#include "../include/buddy_system_message_allocator.h"
#include "../include/broker_logs_manager.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/queue_code_name_associations.h"

char* cache_dump_log_name = "cache_dump.log";

void initialize_broker_logs_manager(){
    initialize_logger_for("Broker");
    create_main_logger_from_config();
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

void log_succesful_new_message_pushed_to_a_queue(uint32_t message_id, uint32_t queue_code){
    char* message = string_from_format("Se pusheo el mensaje con id: %d a la cola de mensajes: %s correctamente!", message_id, queue_name_of(queue_code));
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_succesful_message_sent_to_a_suscriber(uint32_t message_id, t_subscriber_context* subscriber_context){
    char* message = string_from_format("Se envió correctamente el mensaje con id: %d al suscriptor: %s con socket: %d", message_id, subscriber_context -> process_description, subscriber_context -> socket_fd);
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_succesful_message_received_by(t_subscriber_context* subscriber_context, uint32_t message_id){
    char* message = string_from_format("El suscriptor: %s recibió el mensaje con id: %d\n", subscriber_context -> process_description, message_id);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_succesful_save_message_to_cache(uint32_t message_id, uint32_t message_size, void* message_position){

    char* message =
            string_from_format("Se guardó correctamente el mensaje con id: %lu, de tamaño %lu, en la posicion de memoria: %lu",
                                       message_id, message_size, decimal_position_in_memory(message_position));

    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_succesful_free_partition_to_cache(void* message_position,uint32_t message_id){
    char* message = string_from_format("Se liberó correctamente el mensaje con id:'%d' en la posicion de memoria: %lu",message_id, decimal_position_in_memory(message_position));
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

void log_succesful_memory_compaction_as_buddies(void* master_block_position, void* buddy_block_position){
    char* message = string_from_format("Se asoció correctamente los bloques buddy en la posicion de memoria: '%lu' con '%lu' ", decimal_position_in_memory(master_block_position), decimal_position_in_memory(buddy_block_position));
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

void log_received_message_of(uint32_t operation_code){

    char* operation_name = operation_code_as_string(operation_code);
    char* message = string_from_format("El server recibió un mensaje de tipo %s\n", operation_name);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_succesful_message_sent_to_suscribers(uint32_t message_id){
    char* message = string_from_format("El mensaje con id: %d fue enviado correctamente a todos los suscriptores.", message_id);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_succesful_get_and_update_subscribers_to_send(uint32_t message_id){
    char* message = string_from_format("Se actualizaron los suscriptores a enviar del mensaje con id: %d\n",message_id);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_update_of_message_id_received_for(t_subscriber_context* subscriber_context){
    char* message = string_from_format("Se le actualizo al suscriptor: %s su ultimo mensaje id recibido con el id: %zu", subscriber_context -> process_description ,  subscriber_context -> last_message_id_received);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_succesful_deleting_message_of_a_queue(uint32_t message_id, char* reason){
    char* message = string_from_format("Se quitó de la cola el mensaje con id: %d correctamente! Motivo de borrado: %s", message_id, reason);
    log_succesful_message(process_execution_logger(), message);
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

void log_subscriber_not_found_in_message_status_subscribers_error(t_subscriber_context* subscriber_context, uint32_t message_id){

    char* message = string_from_format("No se encontro al suscriptor: %s para removerlo de la lista del mensaje con id: %d", subscriber_context -> process_description, message_id);
    log_errorful_message(process_execution_logger(), message);
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
    char* message = string_from_format("No se pudo encontrar un mensaje con id:'%d' en la cola de mensajes. Ya fue eliminado previamente.",message_id);
    log_warning_message(process_execution_logger(), message);
    free(message);
}

void log_no_subscribers_for_message_with_id(uint32_t message_id){
    char* message = string_from_format("No hay suscriptores en la cola donde se encuentra el mensaje con id:%d", message_id);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_block_information_with_id_not_found(uint32_t message_id){
    char* message = string_from_format("No se encontró un mensaje (memory_block) con el id: %d", message_id);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void free_broker_logs_manager(){
    free_loggers();
}