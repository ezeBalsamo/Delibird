#include <team_manager.h>
#include <map.h>
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/garbage_collector.h"
#include <stdlib.h>
#include <query_performer.h>
#include <appeared_query_performer.h>
#include <localized_query_performer.h>
#include <caught_query_performer.h>
#include <team_logs_manager.h>

t_list* query_performers;

void free_query_performer(){
    list_destroy_and_destroy_elements(query_performers,free);
}

t_request* internal_request_from(t_request* deserialized_request){

    t_identified_message* correlative_identified_message = (t_identified_message*) deserialized_request -> structure;
    t_identified_message* original_identified_message = internal_object_in(correlative_identified_message);

    return original_identified_message -> request;
}

void initialize_query_performer(){
    initialize_appeared_query_performer();
    initialize_localized_query_performer();
    initialize_caught_query_performer();

    query_performers = list_create();

    list_add(query_performers, (void*) appeared_query_performer());
    list_add(query_performers, (void*) localized_query_performer());
    list_add(query_performers, (void*) caught_query_performer());
}

t_query_performer* query_performer_handle(uint32_t operation){

    initialize_query_performer();

    bool _can_handle(void* query_performer){
        t_query_performer* cast_query_performer = (t_query_performer*) query_performer;
        return (*(cast_query_performer -> can_handle_function)) (operation);
    }

    t_query_performer* query_performer_found = list_remove_by_condition(query_performers, _can_handle);

    if (!query_performer_found){
        log_query_performer_not_found_error_for(operation);
        free_system();
    }

    free_query_performer();
    return query_performer_found;
}

void query_perform(t_request* request) {
    t_request* parse_request = internal_request_from(request);

    t_query_performer* query_performer = query_performer_handle(parse_request -> operation);

    query_performer -> perform_function (request -> structure);

}