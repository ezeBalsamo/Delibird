#include "../include/gameboy_connection_handler.h"
#include "../include/gamecard_query_performers.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/general_logs.h"
#include "../../Utils/include/logger.h"
#include <stdlib.h>

char* port(){
    return config_get_string_at("PUERTO");
}

void* main_thread_handler(void* connection_fd){
    int cast_connection_fd = *((int*) connection_fd);

    t_receive_information* receive_information  = receive_structure(cast_connection_fd);

    if(receive_information -> receive_was_successful){

        t_request* deserialized_request = deserialize(receive_information -> serialization_information -> serialized_request);

        //Loguear y mostrar por consola mensaje recibido
        log_request_received_with(main_logger(), deserialized_request);

        //Realizar lógica
        //En este punto tambien debo armar un mensaje con el mismo id que me llego para publicar en la cola corresp
        gamecard_query_perform(deserialized_request);

        free(deserialized_request);
    }

    free_and_close_connection(connection_fd);
    free_receive_information(receive_information);

    return NULL;
}

void* initialize_gamecard_gameboy_connection_handler(){
    start_multithreaded_server(port(), main_thread_handler);

    return NULL;
}

void free_gameboy_connection_handler(){
    free_multithreaded_server();
}