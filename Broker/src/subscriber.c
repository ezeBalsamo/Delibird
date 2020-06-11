#include <queue_context_provider.h>
#include <broker_logs_manager.h>
#include "subscriber.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/configuration_manager.h"

void configure_ack_timeout(int socket_fd){

    int timeout_in_seconds = config_get_int_at("ACK_TIMEOUT");
    configure_socket_timeout_for(socket_fd, timeout_in_seconds);
}

void subscribe_client_to_queue(int subscriber, uint32_t operation_queue){
    t_queue_context* queue_context = get_context_of_operation_queue(operation_queue);
    t_list* subscribers = queue_context -> subscribers;

    int* subscriber_socket_fd = safe_malloc(sizeof(int));
    *subscriber_socket_fd = subscriber;

    configure_ack_timeout(*subscriber_socket_fd);
    pthread_mutex_lock(&(queue_context -> subscribers_mutex));
    list_add(subscribers, (void*) subscriber_socket_fd);
    pthread_mutex_unlock(&(queue_context -> subscribers_mutex));
    log_succesful_subscription_process(subscriber);
}

void revoke_suscription(t_message_status* message_status, t_queue_context* queue_context , int subscriber){

    bool _equals_subscribers(void* another_subscriber){
        return subscriber == *((int*)another_subscriber);
    }

    void* subscriber_found_in_message_status = list_remove_by_condition(message_status -> subscribers_to_send, (bool (*)(void *)) _equals_subscribers);
    void* subscriber_found_in_subscribers = list_remove_by_condition(queue_context -> subscribers, (bool (*)(void *)) _equals_subscribers);

    if(!subscriber_found_in_message_status){
        //TODO log
    }

    if(!subscriber_found_in_subscribers){
        //TODO log
    }

    else {
        int cast_subscriber_found = *((int*)subscriber_found_in_subscribers);
        close(cast_subscriber_found);
        log_subscriber_disconnection(cast_subscriber_found);
    }
}

bool is_still_subscribed(t_queue_context* queue_context, int subscriber){
    bool _equals_subscribers(void* another_subscriber){
        return subscriber == *((int*)another_subscriber);
    }

    return list_contains(queue_context -> subscribers, (void*) &subscriber, (bool (*)(void *, void *)) _equals_subscribers);
}

void send_all_messages(int subscriber, uint32_t operation_queue){
    t_queue_context* queue_context = get_context_of_operation_queue(operation_queue);
    void* subscriber_ack;

    t_list* queue_messages = (queue_context -> queue -> elements);

    for(int i = 0; i < list_size(queue_messages) && is_still_subscribed(queue_context, subscriber); i++){

        t_message_status* message_status = list_get(queue_messages, i);
        t_request* request = create_request_from(message_status);

        serialize_and_send_structure(request, subscriber);

        pthread_t waiting_for_ack_thread = default_safe_thread_create(receive_ack_thread, &subscriber);

        pthread_join(waiting_for_ack_thread, &subscriber_ack);

        uint32_t cast_subscriber_ack = *((uint32_t *) subscriber_ack);

        if (cast_subscriber_ack == FAILED_ACK){
            log_ack_failed_to_received_error();
            revoke_suscription(message_status, queue_context, subscriber);
        } else {
            move_subscriber_to_ACK(message_status, subscriber);
            log_succesful_all_messages_of_a_queue_sent_to(subscriber);
        }
    }
}