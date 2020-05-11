#include "../include/gameboy_connection_handler.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/configuration_manager.h"

#include <stdlib.h>

char* port(){
    return config_get_string_at("PUERTO");
}

void* initialize_gameboy_connection_handler(){
    multithreaded_server_listening_at(port());

    return NULL;
}