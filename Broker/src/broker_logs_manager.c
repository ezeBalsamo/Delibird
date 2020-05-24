#include "../include/broker_logs_manager.h"
#include "../include/broker_deserialization.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/pretty_printer.h"
#include <commons/string.h>
#include <stdlib.h>

void initialize_broker_logs_manager(){
    initialize_logger();
    create_main_logger_for("Broker");
    create_process_execution_logger_for("Broker");
    initialize_pretty_printer();
}

//LOGS MAIN, LOS QUE VAN SI O SI EN EL TP.
//---------------------------------------------------------------------------------------

void log_succesful_connection_of_a_process(){
    char* message = "Se conectó un proceso correctamente.";
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
}

void log_succesful_subscription_process(){
    char* message = "Se suscribió un proceso a una cola de mensajes correctamente.";
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
}

void log_succesful_new_message_pushed_to_a_queue(void* serialized_request){ //aca agarrar void*
    char* message = "Llegó un nuevo mensaje a una cola de mensajes correctamente.";
    log_succesful_message(main_logger(), message);
    t_request* deserialized_request = deserialize(serialized_request);
    char* printed_object = request_pretty_print(deserialized_request);
    log_succesful_message(process_execution_logger(), message);
    log_succesful_message(process_execution_logger(), printed_object);
    free_request(deserialized_request);
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

void log_succesful_start_up(){
    log_succesful_message(process_execution_logger(), "Broker se ha inicializado correctamente!\n");
}

void log_succesful_initialize_config_manager(){
    log_succesful_message(process_execution_logger(), "El configuration_manager se ha inicializado correctamente!\n");
}

void log_succesful_initialize_queue_message_manager(){
    log_succesful_message(process_execution_logger(), "El queue_message_manager se ha inicializado correctamente!\n");
}

void log_succesful_queues_creation(){
    log_succesful_message(process_execution_logger(), "Las colas de mensajes se han creado correctamente!\n");
}

void log_succesful_subscribers_list_creation(){
    log_succesful_message(process_execution_logger(), "La lista de suscriptores se ha creado correctamente!\n");
}

void log_server_initial_status(){
    log_succesful_message(process_execution_logger(), "El server multihilo fue levantado y esta esperando recibir información.\n");
}

void log_structure_received(void* serialized_request){
    log_succesful_message(process_execution_logger(), "El server recibió mensaje y esta listo para ser tratado.\n");
    t_request* deserialized_request = deserialize(serialized_request);
    char* printed_object = request_pretty_print(deserialized_request);
    log_succesful_message(process_execution_logger(), printed_object);
    free_request(deserialized_request);
}

void log_succesful_message_sent_to_suscribers(void* serialized_request){
    log_succesful_message(process_execution_logger(), "El mensaje fue enviado correctamente a todos los suscriptores.\n");
    t_request* deserialized_request = deserialize(serialized_request);
    char* printed_object = request_pretty_print(deserialized_request);
    log_succesful_message(process_execution_logger(), printed_object);
    free_request(deserialized_request);
}

void log_no_subscribers_for_request(void* serialized_request){
    log_succesful_message(process_execution_logger(), "No hay suscriptores en la cola donde se encuentra este mensaje.\n");
    t_request* deserialized_request = deserialize(serialized_request);
    char* printed_object = request_pretty_print(deserialized_request);
    log_succesful_message(process_execution_logger(), printed_object);
    free_request(deserialized_request);
}

void free_broker_logs_manager(){
    free_pretty_printer();
    free_loggers();
}