#include <message_role_identifier.h>
#include <connection_handler.h>
#include <semaphore.h>
#include <queue_message_manager.h>
#include <stdlib.h>
#include "../../Utils/include/socket.h"

t_message_role_identifier* publish_message_mode;

t_message_role_identifier* publish_message_mode_function(){
    return publish_message_mode;
}

bool publish_mode_can_handle(uint32_t operation){
    return operation != SUBSCRIBE_ME;
}

t_message_status* create_message_status_structure(t_identified_message* identified_message){
    t_message_status* message_status = safe_malloc(sizeof(t_message_status));
    message_status -> identified_message = identified_message;
    message_status -> subscribers_to_send = list_create();
    message_status -> subscribers_who_received = list_create();

    return message_status;
}

void consume_message(t_connection_request* connection_request){

    sem_wait(get_mutex_id());
    uint32_t message_id = get_message_id();
    update_message_id();
    sem_post(get_mutex_id());

    t_identified_message* identified_message = create_identified_message(message_id, connection_request -> request);

    send_ack_message(message_id, connection_request -> socket_fd);
    t_message_status* message_status = create_message_status_structure(identified_message);

    push_to_queue(message_status);
}

void initialize_publish_message_mode(){
    publish_message_mode = safe_malloc(sizeof(t_message_role_identifier));
    publish_message_mode -> can_handle_function  = publish_mode_can_handle;
    publish_message_mode -> attending_message_function = consume_message;
}

void free_message_status(t_message_status* message_status){
    free_identified_message(message_status -> identified_message);
    list_destroy_and_destroy_elements(message_status -> subscribers_who_received, free);
    list_destroy_and_destroy_elements(message_status ->subscribers_to_send, free);
}