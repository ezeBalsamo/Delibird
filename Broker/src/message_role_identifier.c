#include <commons/collections/list.h>
#include <stdlib.h>
#include <subscriber_message_mode.h>
#include "../include/message_role_identifier.h"
#include "../../Utils/include/common_structures.h"
#include "../include/publish_message_mode.h"

t_list* message_identifier;

void free_message_identifier(){
    list_destroy_and_destroy_elements(message_identifier, free);
}

void initialize_message_role_identifier(){
    initialize_publish_message_mode();
    initialize_subscriber_message_mode();

    message_identifier = list_create();

    list_add(message_identifier, publish_message_mode_function());
    list_add(message_identifier, subscriber_message_mode_function());
}

t_message_role_identifier* message_role_handler(uint32_t operation){

    initialize_message_role_identifier();

    bool can_handle_ (void* message_role_identifier){
        t_message_role_identifier* cast_message_role_identifier = (t_message_role_identifier*) message_role_identifier;
        return (*(cast_message_role_identifier -> can_handle_function)) (operation);
    }

    t_message_role_identifier* message_role_identifier = list_remove_by_condition(message_identifier, can_handle_);

    if (message_role_identifier == NULL){
        //TODO     log_invalid_operation_to_message_identifier(operation);
    }

    free_message_identifier();

    return message_role_identifier;
}

void message_role_identifier(t_connection_request* connection_request){

    t_message_role_identifier* message_role_identifier = message_role_handler((connection_request -> request) -> operation);
    message_role_identifier -> attending_message_function (connection_request);
}