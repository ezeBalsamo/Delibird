#include <commons/string.h>
#include "../include/entry_point_connection_builder.h"
#include "../include/entry_point_validator.h"
#include "../../Utils/include/configuration_manager.h"
#include <stdlib.h>

void initialize_entry_point_connection_builder(){
    initialize_configuration_manager();
}

t_request* safe_request(){

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = pokemon_operation_code();
    request -> structure = pokemon_operation_structure();
    request -> sanitizer_function = pokemon_operation_sanitizer_function();

    return request;
}

char* process_ip(){

    char* process_name = valid_process_name_for_connection();
    char* process_ip = string_new();
    string_append(&process_ip, "IP_");
    string_append(&process_ip, process_name);
    char* ip = config_get_string_at(process_ip);

    free(process_ip);
    return ip;
}

char* process_port(){

    char* process_name = valid_process_name_for_connection();
    char* process_port = string_new();
    string_append(&process_port, "PUERTO_");
    string_append(&process_port, process_name);
    char* port = config_get_string_at(process_port);

    free(process_port);
    return port;
}

int ack_timeout(){
    return config_get_int_at("ACK_TIMEOUT");
}

void free_entry_point_connection_builder(){
    free_configuration_manager();
}
