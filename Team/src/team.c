#include <stdio.h>
#include <execinfo.h>
#include <zconf.h>
#include "../include/team_manager.h"
#include "../include/map.h"
#include "../include/team_logs_manager.h"
#include "../include/team_operations_information.h"
#include "../include/broker_connection_handler.h"
#include "../include/gameboy_connection_handler.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/pthread_wrapper.h"
#include <stdlib.h>
#include <signal.h>

void handler(int sig) {
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}


int main(void){
    signal(SIGSEGV, handler);
    initialize_team_logs_manager();
    initialize_team_operations_information();
    initialize_configuration_manager_named("team-RR");
    initialize_team_manager();

    initialize_map();
    log_succesful_start_up();

//    pthread_t broker_connection_handler_thread = thread_create(initialize_broker_connection_handler, NULL, default_thread_create_error_response_strategy);
    pthread_t gameboy_connection_handler_thread = thread_create(initialize_gameboy_connection_handler, NULL, default_thread_create_error_response_strategy);

//    thread_join(broker_connection_handler_thread);
    thread_join(gameboy_connection_handler_thread);

    free_team_manager();
}