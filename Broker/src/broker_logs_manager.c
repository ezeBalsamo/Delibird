#include <commons/string.h>
#include <stdlib.h>
#include <subscriber_context_provider.h>
#include "../include/broker_logs_manager.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/queue_code_name_associations.h"

void initialize_broker_logs_manager(){
    initialize_logger_for("Broker");
    create_main_logger();
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
    char* message = string_from_format("Se pusheo el mensaje:\n%s\na la cola de mensajes: %s correctamente!", printed_object, queue_name_of(queue_code));
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

//TODO Faltan implementar logs del main: DEL 5 al 8.

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
    char* message = string_from_format("Se actualizaron los suscriptores a enviar del mensaje:\n%s\n",printed_object);
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

void log_succesful_all_messages_of_a_queue_sent_to(t_subscriber_context* subscriber_context){
    char* message = string_from_format("Se le enviaron correctamente todos los mensajes de la cola: %s al siguiente suscriptor: %s con socket: %d", queue_name_of(subscriber_context ->operation_queue), subscriber_context -> process_description, subscriber_context -> socket_fd);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_update_of_message_id_received_for(t_subscriber_context* subscriber_context){
    char* message = string_from_format("Se le actualizo al suscriptor: %s su ultimo mensaje id recibido con el id: %zu", subscriber_context -> process_description ,  subscriber_context -> last_message_id_received);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_subscriber_disconnection(t_subscriber_context* subscriber_context){
    char* message = string_from_format("Se desconecto al suscriptor: %s con socket: %d de la cola: %s", subscriber_context -> process_description, subscriber_context -> socket_fd, queue_name_of(subscriber_context -> operation_queue));
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_received_unknown_operation_error(){
    log_errorful_message(process_execution_logger(), "No se recibió una operación válida para poder poner en una cola de mensajes.\n");
}

void log_invalid_operation_to_message_role_identifier_error(uint32_t operation){
    char* message = string_from_format("No se encontró un rol que maneje a la siguiente operación: %d.", operation);
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

void log_subscriber_not_found_in_queue_subscribers_warning(t_subscriber_context* subscriber_context, uint32_t queue_code){

    char* process_description = subscriber_context -> process_description;
    char* queue_name = queue_name_of(queue_code);
    char* message =
            string_from_format("No se encontro suscriptor: %s para removerlo de la lista de la siguiente cola de mensajes: %s\n Es un nuevo suscriptor.",
                    process_description, queue_name);
    log_warning_message(process_execution_logger(), message);
    free(message);
}

void free_broker_logs_manager(){
    free_loggers();
}