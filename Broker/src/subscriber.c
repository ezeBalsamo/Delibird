#include <subscriber.h>
#include <pthread.h>
#include <broker_logs_manager.h>
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/pthread_wrapper.h"
#include <commons/string.h>
#include <stdlib.h>

void update_last_message_id_received_for(t_subscriber_context* subscriber_context, uint32_t last_message_id_received){
    subscriber_context -> last_message_id_received = last_message_id_received;
}

void subscribe_client_to_queue(t_subscriber_context* subscriber_context){
    t_queue_context* queue_context = queue_context_with_code(subscriber_context -> operation_queue);

    t_subscriber_context* old_subscriptor = old_suscriptor_of(queue_context, subscriber_context);

    if(!old_subscriptor){
        log_subscriber_not_found_in_queue_subscribers_warning(subscriber_context, queue_context -> operation);
    } else {
        update_last_message_id_received_for(subscriber_context, old_subscriptor -> last_message_id_received);
        log_update_of_message_id_received_for(subscriber_context);
    }

    queue_context -> queue_context_operations -> add_subscriber_function (queue_context, (void*) subscriber_context);
    log_succesful_subscription_process(subscriber_context);
}

bool are_equivalent_subscribers(t_subscriber_context* subscriber_context, t_subscriber_context* another_subscriber_context) {
    return subscriber_context -> operation_queue == another_subscriber_context -> operation_queue &&
           string_equals_ignore_case(subscriber_context -> process_description,
                                     another_subscriber_context -> process_description);
}

void add_subscriber_to_all_messages_status_subscribers_to_send_list(t_list* messages_to_send, t_subscriber_context* subscriber_context){

    void _add_subscriber_to_send_if_necessary(t_message_status* message_status){

        t_list* subscribers_to_send = message_status -> subscribers_to_send;

        if(!list_contains(subscribers_to_send, subscriber_context, (bool (*) (void*, void*)) are_equivalent_subscribers)){
            list_add(subscribers_to_send, (void*) subscriber_context);
        }
    }

    list_iterate(messages_to_send, (void (*) (void*)) _add_subscriber_to_send_if_necessary);
}

void send_all_messages(t_subscriber_context* subscriber_context) {
    t_queue_context* queue_context = queue_context_with_code(subscriber_context -> operation_queue);
    t_list* queue_messages = queue_context -> messages;

    bool _has_to_be_send(t_message_status* message_status){
        return message_status -> identified_message -> message_id > subscriber_context -> last_message_id_received;
    }

    t_list* messages_to_send = list_filter(queue_messages, (bool (*)(void*)) _has_to_be_send);
    add_subscriber_to_all_messages_status_subscribers_to_send_list(messages_to_send, subscriber_context);

    uint32_t ack;
    for(int i = 0; i < list_size(messages_to_send) && ack != FAILED_ACK; i++) {

        t_message_status* message_status = list_get(messages_to_send, i);

        t_request* request = create_request_from(message_status);
        serialize_and_send_structure(request, subscriber_context -> socket_fd);

        pthread_t waiting_for_ack_thread = default_safe_thread_create(receive_ack_thread, (void*) &subscriber_context -> socket_fd);

        void* ack_result = join_reception_for_ack_thread(waiting_for_ack_thread, subscriber_context, message_status);
        ack = *((uint32_t *) ack_result);
        free(ack_result);
    }
    list_destroy(messages_to_send);
}