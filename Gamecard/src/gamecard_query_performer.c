#include "../../Utils/include/common_structures.h"
#include "filesystem.h"
#include <gamecard_logs_manager.h>
#include <gamecard_query_performer.h>
#include <new_query_performer.h>
#include <catch_query_performer.h>
#include <get_query_performer.h>
#include <stdio.h>
#include <stdlib.h>

t_list* query_performers;

void free_query_performer(){
    list_destroy_and_destroy_elements(query_performers,free);
}

t_request* internal_request_in(t_request* deserialized_request){

    t_identified_message* original_identified_message = (t_identified_message*) deserialized_request -> structure;

    return original_identified_message -> request;
}

void initialize_query_performer(){
    initialize_new_query_performer();
    initialize_catch_query_performer();
    initialize_get_query_performer();
    query_performers = list_create();

    list_add(query_performers, (void*) new_query_performer());
    list_add(query_performers, (void*) catch_query_performer());
    list_add(query_performers, (void*) get_query_performer());
}

t_gamecard_query_performer* query_performer_handle(uint32_t operation){

    initialize_query_performer();

    bool _can_handle(void* query_performer){
        t_gamecard_query_performer* cast_query_performer = (t_gamecard_query_performer*) query_performer;
        return (*(cast_query_performer -> can_be_handled_function)) (operation);
    }

    //Me quedo con el Query Performer que puede manejar el mensaje que me llegó
    t_gamecard_query_performer* query_performer = list_remove_by_condition(query_performers,_can_handle);
    if (query_performer == NULL){
        log_invalid_operation_to_query_performer_from_gamecard(operation);
    }
    free_query_performer();
    return query_performer;
}

void gamecard_query_perform(t_request* request) {
    //Parseo de la request deserializada (indentified message x2) al mensaje en si
    t_request* parse_request = internal_request_in(request);

    //Obtengo el Query Performer capaz de manejar el mensaje que me llegó
    //t_gamecard_query_performer* query_performer = query_performer_handle(parse_request->operation);
    t_gamecard_query_performer* query_performer = query_performer_handle(parse_request->operation);

    //Ejecuto la función que tiene la lógica implementada para el mensaje que me llegó
    query_performer->perform_function (request->structure);

}