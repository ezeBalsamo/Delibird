#include <commons/string.h>
#include <fcntl.h>
#include <broker_memory_manager.h>
#include "initialize_signal_broker_handler.h"
#include <signal.h>
#include <commons/log.h>
#include <stdlib.h>

void cache_handler(){
    t_log* cache_dump_logger = log_create("../logs/cache_dump.log","BROKER",false,LOG_LEVEL_INFO);
    char* cache_info = dump_cache();
    log_info(cache_dump_logger, cache_info);
    free(cache_info);
}


void initialize_broker_signal_handler(){
    initialize_signal_handler();
    handle_signal(SIGUSR1,cache_handler);
}


