#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/general_logs.h"
#include "../include/gamecard_logs_manager.h"
#include "../include/gamecard_serializable_objects.h"
#include "../include/broker_connection_handler.h"
#include "../include/gameboy_connection_handler.h"
#include "../include/gamecard_configuration_manager.h"
#include "../include/gamecard_query_performers.h"
#include <stdlib.h>
#include "open_files_structure.h"

#include "../../Utils/include/pthread_wrapper.h"

void* free_system_debugging_thread(){
    sleep_for(45);
    free_system();
    return NULL;
}

int main(void) {

    initialize_signal_handler();
    initialize_garbage_collector();
    initialize_gamecard_configuration_manager();

    initialize_gamecard_logs_manager();
    initialize_pretty_printer();
    initialize_gamecard_serializable_objects();
    initialize_gamecard_query_performers();
    initialize_file_system();
    initialize_open_files_list();

    log_succesful_start_up();

    initialize_gamecard_broker_connection_handler();

    pthread_t debugging_tid = default_safe_thread_create(free_system_debugging_thread, NULL);

    initialize_gamecard_gameboy_connection_handler();

	return EXIT_SUCCESS;
}
