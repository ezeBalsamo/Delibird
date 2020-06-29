#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/garbage_collector.h"
#include "file_system.h"
#include <gamecard_logs_manager.h>
#include <gamecard_query_performers.h>
#include <new_query_performer.h>
#include <catch_query_performer.h>
#include <get_query_performer.h>
#include <stdio.h>
#include <stdlib.h>

t_list* query_performers;

void free_gamecard_query_performers(){
    list_destroy_and_destroy_elements(query_performers,free);
}

t_request* internal_request_in(t_request* deserialized_request){

    t_identified_message* original_identified_message = (t_identified_message*) deserialized_request -> structure;

    return original_identified_message -> request;
}

void initialize_gamecard_query_performers(){
    initialize_new_query_performer();
    initialize_catch_query_performer();
    initialize_get_query_performer();
    query_performers = list_create();

    list_add(query_performers, (void*) new_query_performer());
    list_add(query_performers, (void*) catch_query_performer());
    list_add(query_performers, (void*) get_query_performer());
}

t_gamecard_query_performer* query_performer_handle(uint32_t operation){

    bool _can_handle(void* query_performer){
        t_gamecard_query_performer* cast_query_performer = (t_gamecard_query_performer*) query_performer;
        return (*(cast_query_performer -> can_be_handled_function)) (operation);
    }

    //Me quedo con el Query Performer que puede manejar el mensaje que me llegó
    t_gamecard_query_performer* query_performer_found = list_find(query_performers, _can_handle);

    //Si no se encontró un Query Performer capaz de manejarlo, me llegó una operacion que no entiendo, logueo y rompo
    if (!query_performer_found){
        log_query_performer_not_found_error_from_gamecard_for(operation);
        free_system();
    }

    return query_performer_found;
}

t_identified_message* gamecard_query_perform(t_request* request) {
    //Parseo de la request deserializada (indentified message x2) al mensaje en si
    t_request* parse_request = internal_request_in(request);

    //Obtengo el Query Performer capaz de manejar el mensaje que me llegó
    t_gamecard_query_performer* query_performer = query_performer_handle(parse_request->operation);

    //Ejecuto la función que tiene la lógica implementada para el mensaje que me llegó
    return query_performer->perform_function (request->structure);

}