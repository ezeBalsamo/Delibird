#include <stdlib.h>
#include <subscriber.h>
#include <broker_memory_manager.h>
#include <broker_signal_handler.h>
#include <main_thread_executor.h>
#include "../../Utils/include/configuration_manager.h"
#include "../include/broker_logs_manager.h"
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/garbage_collector.h"

int main() {
    initialize_broker_signal_handler();
    initialize_garbage_collector();
    initialize_configuration_manager();
    initialize_messages_roles();
    initialize_broker_logs_manager();
    initialize_pretty_printer();
    initialize_serializable_objects();
    initialize_queue_context_provider();
    initialize_broker_memory_manager();
    initialize_main_thread_executor();

    log_succesful_start_up();

    execute_main_thread();

    return EXIT_SUCCESS;
}
