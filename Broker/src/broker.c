#include "../include/queue_message_manager.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../include/connection_handler.h"
#include "../../Utils/include/configuration_manager.h"

int main() {

    initialize_configuration_manager_named("broker");
    initialize_queue_message_manager();

    pthread_t connection_handler_thread = default_safe_thread_create(initialize_connection_handler, NULL);

    thread_join(connection_handler_thread);

}
