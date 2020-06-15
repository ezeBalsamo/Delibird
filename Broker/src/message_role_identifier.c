#include <commons/collections/list.h>
#include <stdlib.h>
#include <subscriber_message_mode.h>
#include <broker_logs_manager.h>
#include "../include/message_role_identifier.h"
#include "../../Utils/include/common_structures.h"
#include "../include/publisher_message_mode.h"
#include "../../Utils/include/garbage_collector.h"

t_list* messages_modes;

void free_message_identifier(){
    list_destroy_and_destroy_elements(messages_modes, free);
}

void initialize_message_role_identifier(){
    messages_modes = list_create();

    initialize_publisher_message_mode();
    initialize_subscriber_message_mode();

    list_add(messages_modes, publisher_mode());
    list_add(messages_modes, subscriber_mode());
}

t_message_role_identifier* find_message_role_identifier(uint32_t operation){

    bool _can_handle (void* message_role_identifier){
        t_message_role_identifier* cast_message_role_identifier = (t_message_role_identifier*) message_role_identifier;
        return (*(cast_message_role_identifier -> can_handle_function)) (operation);
    }

    t_message_role_identifier* message_role_identifier = list_find(messages_modes, _can_handle);

    if (message_role_identifier == NULL){
        log_invalid_operation_to_message_role_identifier_error(operation);
        free_system();
    }

    return message_role_identifier;
}

void attend_with_message_role_identifier(t_connection_request* connection_request){

    t_message_role_identifier* message_role_identifier = find_message_role_identifier(connection_request -> request -> operation);
    message_role_identifier -> attending_message_function (connection_request);
}