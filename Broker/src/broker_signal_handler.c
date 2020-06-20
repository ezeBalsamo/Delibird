#include <signal.h>
#include "../include/broker_signal_handler.h"
#include "../../Utils/include/common_structures.h"
#include <commons/log.h>
#include <broker_memory_manager.h>
#include <stdlib.h>

void cache_handler(){
    t_log* cache_dump_logger = log_create("../logs/cache_dump.log","BROKER",false,LOG_LEVEL_INFO);
    char* cache_info = dump_cache();
    log_info(cache_dump_logger, cache_info);
    free(cache_info);
}

void initialize_broker_signal_handler(){
    initialize_signal_handler();
    signal(SIGPIPE, SIG_IGN);
    handle_signal(SIGUSR1,cache_handler);
}
