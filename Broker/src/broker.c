#include "../include/queue_message_manager.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../include/connection_handler.h"
#include "../../Utils/include/configuration_manager.h"
#include "../include/broker_logs_manager.h"
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/pretty_printer.h"

int main() {

    initialize_broker_logs_manager();
    initialize_pretty_printer();
    initialize_serializable_objects();
    initialize_configuration_manager_named("broker");

    log_succesful_initialize_config_manager();

    initialize_queue_message_manager();

    log_succesful_initialize_queue_message_manager();

    pthread_t connection_handler_thread = default_safe_thread_create(initialize_connection_handler, NULL);

    thread_join(connection_handler_thread);

    free_broker_logs_manager();
    free_pretty_printer();
    free_serializable_objects();
    free_queue_message_manager();
    free_configuration_manager();

    return 0;
}
