#include <subscriber.h>
#include <pthread.h>
#include <broker_logs_manager.h>
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/pthread_wrapper.h"
#include <commons/string.h>

void configure_ack_timeout_for(int socket_fd){

    int timeout_in_seconds = config_get_int_at("ACK_TIMEOUT");
    configure_socket_timeout_in_seconds(socket_fd, timeout_in_seconds);
}

void subscribe_client_to_queue(t_subscriber_context* subscriber_context){
    t_queue_context* queue_context = queue_context_with_code(subscriber_context -> operation_queue);

    int socket_fd = subscriber_context -> socket_fd;

    configure_ack_timeout_for(socket_fd);
    queue_context -> queue_context_operations -> add_subscriber_function (queue_context, (void*) &socket_fd);
    log_succesful_subscription_process(subscriber_context);
}

void revoke_suscription(t_message_status* message_status, t_queue_context* queue_context , t_subscriber_context* subscriber_context){

    bool _are_equals_subscribers(t_subscriber_context* subscriber_to_compare){
        return subscriber_context -> operation_queue == subscriber_to_compare -> operation_queue &&
               subscriber_context -> socket_fd == subscriber_to_compare -> socket_fd &&
               subscriber_context -> last_message_id_received == subscriber_to_compare -> last_message_id_received &&
               string_equals_ignore_case(subscriber_context -> process_id, subscriber_to_compare -> process_id);
    }

    void* subscriber_found_in_message_status = list_remove_by_condition((message_status -> subscribers_to_send), (bool (*)(void *)) _are_equals_subscribers);
    void* subscriber_found_in_subscribers = list_remove_by_condition(queue_context -> subscribers, (bool (*)(void *)) _are_equals_subscribers);

    if(!subscriber_found_in_message_status){
        log_no_subscriber_found_in_message_status_subscribers_list_error(message_status -> identified_message);
    }

    if(!subscriber_found_in_subscribers){
        log_no_subscriber_found_in_queue_subscribers_list_error(queue_context -> operation);
        }

    else {
        int subscriber_socket_fd = ((t_subscriber_context*) subscriber_found_in_subscribers) -> socket_fd;
        close(subscriber_socket_fd);
        log_subscriber_disconnection(subscriber_context);
    }
}

bool is_still_subscribed(t_queue_context* queue_context, t_subscriber_context* subscriber_context){

    bool _is_subscribed(t_subscriber_context* subscriber_to_find, t_subscriber_context* subscriber_to_compare){
       return subscriber_to_find -> operation_queue == subscriber_to_compare -> operation_queue &&
              subscriber_to_find -> socket_fd == subscriber_to_compare -> socket_fd &&
              subscriber_to_find -> last_message_id_received == subscriber_to_compare -> last_message_id_received &&
              string_equals_ignore_case(subscriber_to_find -> process_id, subscriber_to_compare -> process_id);
    }

    return list_contains(queue_context -> subscribers, subscriber_context, (bool (*)(void *, void*)) _is_subscribed);
}

void send_all_messages(t_subscriber_context* subscriber_context) {
    t_queue_context* queue_context = queue_context_with_code(subscriber_context -> operation_queue);
    t_list* queue_messages = (queue_context -> queue -> elements);

    for(int i = 0; i < list_size(queue_messages) && is_still_subscribed(queue_context, subscriber_context); i++) {

        t_message_status* message_status = list_get(queue_messages, i);

            if(message_status -> identified_message -> message_id <= subscriber_context -> last_message_id_received){
                continue;
            }

        t_request* request = create_request_from(message_status);
        serialize_and_send_structure(request, subscriber_context -> socket_fd);

        pthread_t waiting_for_ack_thread = default_safe_thread_create(receive_ack_thread, (void*) &subscriber_context -> socket_fd);

        join_reception_for_ack_thread(waiting_for_ack_thread, subscriber_context, message_status, queue_context);
    }
}