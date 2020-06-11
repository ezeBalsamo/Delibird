#include <stdlib.h>
#include <free_broker.h>
#include <pthread.h>
#include <broker_logs_manager.h>
#include <subscriber.h>
#include "../include/queue_message_status.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/socket.h"
#include "commons/string.h"

t_message_status* create_message_status_for(t_identified_message* identified_message){
    t_message_status* message_status = safe_malloc(sizeof(t_message_status));
    message_status -> identified_message = identified_message;
    message_status -> subscribers_to_send = list_create();
    message_status -> subscribers_who_received = list_create();

    return message_status;
}

t_request* create_request_from(t_message_status* message_status){

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = IDENTIFIED_MESSAGE;
    request -> structure = message_status -> identified_message;
    request -> sanitizer_function = (void (*)(void *)) free_identified_message;

    return request;
}

void join_reception_for_ack_thread(pthread_t waiting_for_ack_thread, t_subscriber_context* subscriber_context , t_message_status* message_status, t_queue_context* queue_context){
    void *subscriber_ack;
    int socket_fd = subscriber_context -> socket_fd;

    pthread_join(waiting_for_ack_thread, &subscriber_ack);

    uint32_t cast_subscriber_ack = *((uint32_t *) subscriber_ack);

    if (cast_subscriber_ack == FAILED_ACK){
        log_failed_to_receive_ack_error(subscriber_context);
        revoke_suscription(message_status, queue_context, subscriber_context);
    } else {
        move_subscriber_to_ACK(message_status, subscriber_context);
        log_succesful_all_messages_of_a_queue_sent_to(subscriber_context -> process_id);
    }
}

void move_subscriber_to_ACK(t_message_status* message_status, t_subscriber_context* subscriber_context){

    bool _are_equals_subscribers(t_subscriber_context* subscriber_to_compare){
        return subscriber_context -> operation_queue == subscriber_to_compare -> operation_queue &&
               subscriber_context -> socket_fd == subscriber_to_compare -> socket_fd &&
               subscriber_context -> last_message_id_received == subscriber_to_compare -> last_message_id_received &&
               string_equals_ignore_case(subscriber_context -> process_id, subscriber_to_compare -> process_id);
    }

    void* subscriber_found =
            list_remove_by_condition(message_status -> subscribers_to_send, (bool (*)(void *)) _are_equals_subscribers);

    if(!subscriber_found){
        log_no_subscriber_found_in_message_status_subscribers_list_error(message_status -> identified_message);
        free_system();
    }
    list_add(message_status -> subscribers_who_received, (void*) subscriber_found);
}

void free_message_status(t_message_status* message_status){
    free_identified_message(message_status -> identified_message);
    list_destroy_and_destroy_elements(message_status -> subscribers_who_received, free);
    list_destroy_and_destroy_elements(message_status ->subscribers_to_send, free);
    free(message_status);
}