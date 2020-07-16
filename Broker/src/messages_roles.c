#include <commons/collections/list.h>
#include <stdlib.h>
#include <subscriber_message_mode.h>
#include <broker_logs_manager.h>
#include <connection_handler.h>
#include "../include/messages_roles.h"
#include "../../Utils/include/common_structures.h"
#include "../include/publisher_message_mode.h"
#include "../../Utils/include/garbage_collector.h"

t_list* messages_roles;

void free_message_identifier(){
    list_destroy_and_destroy_elements(messages_roles, free);
}

void initialize_messages_roles(){
    messages_roles = list_create();

    initialize_publisher_message_mode();
    initialize_subscriber_message_mode();

    list_add(messages_roles, publisher_mode());
    list_add(messages_roles, subscriber_mode());
}

t_message_role* find_message_role(uint32_t operation){

    bool _can_handle (void* message_role){
        t_message_role* cast_message_role = (t_message_role*) message_role;
        return (*(cast_message_role -> can_handle_function)) (operation);
    }

    t_message_role* message_role = list_find(messages_roles, _can_handle);

    if (message_role == NULL){
        log_invalid_operation_to_message_role_identifier_error(operation);
        free_system();
    }

    return message_role;
}

void attend_with_message_role(t_connection_deserialization_information* connection_deserialization_information){

    uint32_t operation = connection_deserialization_information -> deserialization_information -> operation;

    t_message_role* message_role = find_message_role(operation);
    message_role -> attending_message_function (connection_deserialization_information);
}