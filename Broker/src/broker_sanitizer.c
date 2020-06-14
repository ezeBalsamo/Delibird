#include <queue_context_provider.h>
#include <broker_logs_manager.h>
#include <stdlib.h>
#include <connection_handler.h>
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/garbage_collector.h"

void free_system(){
    free_pretty_printer();
    free_serializable_objects();
    free_queue_context_provider();
    free_configuration_manager();
    free_garbage_collector();
    free_connection_handler();

    log_successful_clean_up();
    free_broker_logs_manager();

    exit(EXIT_SUCCESS);
}