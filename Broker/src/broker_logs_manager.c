#include <commons/string.h>
#include <stdlib.h>
#include "../include/broker_logs_manager.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/serialization_interface.h"

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

void log_succesful_subscription_process(int subscriber){
    char* message = string_from_format("%s: %d.", "Se suscribió al siguiente proceso a una cola de mensajes correctamente", subscriber);
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_succesful_new_message_pushed_to_a_queue(t_identified_message* identified_message){
    char* message = "Se pusheo un nuevo mensaje a una cola de mensajes correctamente.";
    log_succesful_message(main_logger(), message);
    char* printed_object = request_pretty_print(identified_message -> request);
    log_succesful_message(process_execution_logger(), message);
    log_succesful_message(process_execution_logger(), printed_object);
}

void log_succesful_message_sent_to_a_suscriber(void* serialized_request){
    char* message = "Se envió un mensaje a un suscriptor correctamente.";
    log_succesful_message(main_logger(), message);
    t_request* deserialized_request = deserialize(serialized_request);
    char* printed_object = request_pretty_print(deserialized_request);
    log_succesful_message(process_execution_logger(), message);
    log_succesful_message(process_execution_logger(), printed_object);
    free_request(deserialized_request);
}

//TODO Faltan implementar logs del main: DEL 5 al 8.

//---------------------------------------------------------------------
//LOGS PARA NOSOTROS, PARA CONTROL
//---------------------------------------------------------------------

void log_succesful_initialize_queue_message_manager(){
    log_succesful_message(process_execution_logger(), "El queue_message_manager se ha inicializado correctamente!\n");
}

void log_succesful_initialize_subscriber_manager(){
    log_succesful_message(process_execution_logger(), "El subscriber_manager se ha inicializado correctamente!\n");
}

void log_server_initial_status(){
    log_succesful_message(process_execution_logger(), "El server multihilo fue levantado y esta esperando recibir información.\n");
}

void log_structure_received(void* serialized_request){
    log_succesful_message(process_execution_logger(), "El server recibió mensaje y esta listo para ser tratado.\n");
    char* printed_object = request_pretty_print(serialized_request);
    log_succesful_message(process_execution_logger(), printed_object);
}

void log_succesful_message_sent_to_suscribers(t_identified_message* identified_message){
    log_succesful_message(process_execution_logger(), "El mensaje fue enviado correctamente a todos los suscriptores:");
    char* printed_object = request_pretty_print(identified_message -> request);
    log_succesful_message(process_execution_logger(), printed_object);
}

void log_succesful_get_and_update_subscribers_to_send(){
    log_succesful_message(process_execution_logger(), "Se actualizaron los suscriptores a enviar correctamente.");
}

void log_no_subscribers_for_request(t_identified_message* identified_message){
    log_succesful_message(process_execution_logger(), "No hay suscriptores en la cola donde se encuentra este mensaje:");
    char* printed_object = request_pretty_print(identified_message -> request);
    log_succesful_message(process_execution_logger(), printed_object);
}

void log_succesful_all_messages_of_a_queue_sent_to(int subscriber){
    char* message = string_from_format("%s: %d", "Se le enviaron todos los mensajes de una cola correctamente al siguiente suscriptor", subscriber);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_received_unknown_operation_error(){
    log_errorful_message(process_execution_logger(), "No se recibió una operación válida para poder poner en una cola de mensajes.\n");
}

void log_invalid_operation_to_message_role_identifier_error(uint32_t operation){
    char* message = string_from_format("%s: %d", "No se encontro un rol que maneje a la siguiente operacion", operation);
    log_errorful_message(process_execution_logger(),message);
    free(message);
}

void log_ack_received_error(){
    log_errorful_message(process_execution_logger(), "Se recibio un dato que no es igual al ack que envie.");
}

void free_broker_logs_manager(){
    free_loggers();
}