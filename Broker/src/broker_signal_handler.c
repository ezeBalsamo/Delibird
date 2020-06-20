#include <signal.h>
#include "../include/broker_signal_handler.h"
#include "../../Utils/include/common_structures.h"
#include <broker_memory_manager.h>
#include <stdlib.h>
#include <broker_logs_manager.h>

void cache_handler(){
    char* cache_info = dump_cache();
    log_cache_dump_information(cache_info);
    free(cache_info);
}

void initialize_broker_signal_handler(){
    initialize_signal_handler();
    signal(SIGPIPE, SIG_IGN);
    handle_signal(SIGUSR1,cache_handler);
}
