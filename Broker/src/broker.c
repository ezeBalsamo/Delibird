#include "../include/queue_message_manager.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../include/connection_handler.h"
#include "../../Utils/include/configuration_manager.h"
#include "../include/broker_logs_manager.h"
#include "../../Utils/include/operations_information.h"

int main() {

    initialize_broker_logs_manager();
    initialize_operations_information();
    initialize_configuration_manager_named("broker");

    log_succesful_initialize_config_manager();

    initialize_queue_message_manager();

    log_succesful_initialize_queue_message_manager();

    pthread_t connection_handler_thread = default_safe_thread_create(initialize_connection_handler, NULL);

    thread_join(connection_handler_thread);

}
