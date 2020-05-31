#include <stdlib.h>
#include "../include/queue_message_manager.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../include/connection_handler.h"
#include "../../Utils/include/configuration_manager.h"
#include "../include/broker_logs_manager.h"
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/general_logs.h"

int main() {

    initialize_logs_manager_for("Broker");

    initialize_pretty_printer();
    log_succesful_initialize_pretty_printer();

    initialize_serializable_objects();
    log_succesful_initialize_serializable_objects();

    initialize_configuration_manager_named("broker");
    log_succesful_initialize_config_manager();

    initialize_queue_message_manager();
    log_succesful_initialize_queue_message_manager();

    log_succesful_start_up();

    pthread_t connection_handler_thread = default_safe_thread_create(initialize_connection_handler, NULL);
    thread_join(connection_handler_thread);
    log_successful_execution();

    free_pretty_printer();
    free_serializable_objects();
    free_queue_message_manager();
    free_configuration_manager();
    log_successful_clean_up();
    free_broker_logs_manager();

    return EXIT_SUCCESS;
}
