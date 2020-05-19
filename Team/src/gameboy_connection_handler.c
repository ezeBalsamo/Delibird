#include "../include/gameboy_connection_handler.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/configuration_manager.h"
#include <stdlib.h>

char* port(){
    return config_get_string_at("PUERTO");
}

void* main_thread_handler(void* serialized_request){
    //TODO: lógica al recibir
    return deserialize(serialized_request);
}

void* initialize_gameboy_connection_handler(){
    start_multithreaded_server(port(), main_thread_handler);

    return NULL;
}