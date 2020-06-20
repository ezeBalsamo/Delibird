#include "../include/gamecard_sanitizer.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/general_logs.h"
#include "../include/filesystem.h"
#include "../include/gamecard_logs_manager.h"
#include "../include/gamecard_serializable_objects.h"
#include "../include/broker_connection_handler.h"
#include "../include/gameboy_connection_handler.h"
#include "../include/gamecard_configuration_manager.h"

#include <stdlib.h>


int main(void) {

    //Inicialización
    initialize_signal_handler();
    initialize_garbage_collector();

    initialize_gamecard_logs_manager();
    initialize_pretty_printer();
    initialize_gamecard_serializable_objects();
    initialize_gamecard_configuration_manager();
    //TODO: Inicializar FS con funcion: initialize_filesystem();

    log_succesful_start_up();

    //Levanto hilo para la conexión con el Broker
    pthread_t broker_connection_handler_thread = default_safe_thread_create(initialize_gamecard_broker_connection_handler, NULL);

    //Levanto hilo para la conexión con el Gameboy
    pthread_t gameboy_connection_handler_thread = default_safe_thread_create(initialize_gamecard_gameboy_connection_handler, NULL);

    safe_thread_join(broker_connection_handler_thread);
    safe_thread_join(gameboy_connection_handler_thread);

	return EXIT_SUCCESS;
}