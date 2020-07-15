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

void publisher_mode_attending_message_function(t_connection_request* connection_request){

    uint32_t message_id = update_and_get_message_id();
    t_identified_message* identified_message = create_identified_message(message_id, connection_request -> request);
    allocate_message(identified_message);
    send_ack_message(message_id, connection_request -> socket_fd);
    t_message_status* message_status = create_message_status_for(identified_message);

    push_to_queue(message_status);

    free(connection_request);
    free_identified_message(identified_message);
}

void initialize_publisher_message_mode(){
    publisher_message_mode = safe_malloc(sizeof(t_message_role));
    publisher_message_mode -> can_handle_function  = publisher_mode_can_handle;
    publisher_message_mode -> attending_message_function = publisher_mode_attending_message_function;
}