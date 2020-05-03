#include <commons/string.h>
#include "../include/entry_point_connection_builder.h"
#include "../include/entry_point_validator.h"
#include "../../Utils/include/configuration_manager.h"
#include <stdlib.h>

void initialize_entry_point_connection_builder(){
    initialize_configuration_manager_named("gameboy");
}

t_request* safe_request(){

    t_operation_information* chosen_operation = valid_chosen_operation();

    t_request* request = malloc(sizeof(t_request));
    request -> operation =  chosen_operation -> code;
    request -> structure = (void*) operation_arguments();

    free_operation_information(chosen_operation);
    return request;
}

char* process_ip(){

    t_process_information* process_information = valid_chosen_process();
    char* process_ip = string_new();
    string_append(&process_ip, "IP_");
    string_append(&process_ip, process_information -> name);
    char* ip = config_get_string_at(process_ip);

    free_process_information(process_information);
    free(process_ip);
    return ip;
}

char* process_port(){

    t_process_information* process_information = valid_chosen_process();
    char* process_port = string_new();
    string_append(&process_port, "PUERTO_");
    string_append(&process_port, process_information -> name);
    char* port = config_get_string_at(process_port);

    free_process_information(process_information);
    free(process_port);
    return port;
}

void free_entry_point_connection_builder(){
    free_configuration_manager();
}
