#include <stdlib.h>
#include <subscribers_manager.h>
#include "../include/queue_message_manager.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../include/connection_handler.h"
#include "../../Utils/include/configuration_manager.h"
#include "../include/broker_logs_manager.h"
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/general_logs.h"
#include "free_broker.h"

int main() {

    initialize_signal_handler();
    initialize_broker_logs_manager();
    initialize_pretty_printer();
    initialize_serializable_objects();
    initialize_configuration_manager_named("broker");
    initialize_queue_message_manager();

    log_succesful_start_up();

    pthread_t connection_handler_thread = default_safe_thread_create(initialize_connection_handler, NULL);
    thread_join(connection_handler_thread);
    log_successful_execution();

    free_system();

    return EXIT_SUCCESS;
}
