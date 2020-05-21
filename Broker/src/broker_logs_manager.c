#include "../include/broker_logs_manager.h"
#include "../../Utils/include/logger.h"
#include <commons/string.h>

void initialize_broker_logs_manager(){
    initialize_logger();
    create_main_logger_for("Broker");
    create_process_execution_logger_for("Broker");
}

//LOGS MAIN, LOS QUE VAN SI O SI EN EL TP.
//---------------------------------------------------------------------------------------

void log_succesful_connection_of_a_process(){
    char* message = string_new();
    string_append(&message, "Se conecto un proceso correctamente.");
    log_succesful_message(main_logger(), message);
}

void log_succesful_subscription_process(){
    char* message = string_new();
    string_append(&message, "Se suscribió un proceso a una cola de mensajes correctamente.");
    log_succesful_message(main_logger(), message);
}

void log_succesful_new_message_pushed_to_a_queue(){
    char* message = string_new();
    string_append(&message, "Llegó un nuevo mensaje a una cola de mensajes correctamente.");
    log_succesful_message(main_logger(), message);
}

void log_succesful_send_message_to_a_suscriber(){
    char* message = string_new();
    string_append(&message, "Se envió un mensaje a un suscriptor correctamente.");
    log_succesful_message(main_logger(), message);
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

void log_server_first_status(){
    log_succesful_message(process_execution_logger(), "El server multihilo fue levantado y esta esperando recibir información.\n");
}

void log_structure_recieved(){
    log_succesful_message(process_execution_logger(), "El server recibió mensaje y esta listo para ser tratado.\n");
}

void log_succesful_subscription(){
    log_succesful_message(process_execution_logger(), "El suscriptor fue suscripto a la cola indicada correctamente.\n");
}

void log_pushed_message(){
    log_succesful_message(process_execution_logger(), "El mensaje fue encolado correctamente y esta listo para ser enviado a los suscriptores de esa cola.\n");
}

void log_published_message(){
    log_succesful_message(process_execution_logger(), "El mensaje fue enviado a los suscriptores correctamente.\n");
}

void free_broker_logs_manager(){
    free_loggers();
}