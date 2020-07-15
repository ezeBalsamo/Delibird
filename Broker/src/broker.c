#include <stdlib.h>
#include <subscriber.h>
#include <broker_memory_manager.h>
#include <broker_signal_handler.h>
#include "../../Utils/include/pthread_wrapper.h"
#include "../include/connection_handler.h"
#include "../../Utils/include/configuration_manager.h"
#include "../include/broker_logs_manager.h"
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/garbage_collector.h"

int main() {
    initialize_broker_signal_handler();
    initialize_garbage_collector();
    initialize_messages_roles();
    initialize_broker_logs_manager();
    initialize_pretty_printer();
    initialize_serializable_objects();
    initialize_configuration_manager();
    initialize_queue_context_provider();
    initialize_broker_memory_manager();
    log_succesful_start_up();

    pthread_t* connection_handler_thread = malloc(sizeof(pthread_t));
    consider_as_garbage(connection_handler_thread, (void (*)(void *)) (safe_thread_pointer_cancel));
    *connection_handler_thread = default_safe_thread_create(initialize_connection_handler, NULL);

    safe_thread_join(*connection_handler_thread);
    log_successful_execution();
    free_system();

    return EXIT_SUCCESS;
}
