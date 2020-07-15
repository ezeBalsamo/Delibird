#include <messages_roles.h>
#include <connection_handler.h>
#include <publisher.h>
#include <publisher_message_mode.h>
#include <broker_memory_manager.h>
#include <stdlib.h>
#include "../../Utils/include/socket.h"

t_message_role* publisher_message_mode;

t_message_role* publisher_mode(){
    return publisher_message_mode;
}

bool publisher_mode_can_handle(uint32_t operation){
    return operation != SUBSCRIBE_ME;
}

void publisher_mode_attending_message_function(t_connection_deserialization_information* connection_deserialization_information){

    t_deserialization_information* deserialization_information = connection_deserialization_information -> deserialization_information;

    uint32_t message_id = update_and_get_message_id();
    allocate_message_using(message_id, deserialization_information);
    send_ack_message(message_id, connection_deserialization_information -> socket_fd);
    t_message_status* message_status = create_message_status_using(message_id, deserialization_information);

    push_to_queue(message_status);

    free_connection_deserialization_information(connection_deserialization_information);
}

void initialize_publisher_message_mode(){
    publisher_message_mode = safe_malloc(sizeof(t_message_role));
    publisher_message_mode -> can_handle_function  = publisher_mode_can_handle;
    publisher_message_mode -> attending_message_function = publisher_mode_attending_message_function;
}