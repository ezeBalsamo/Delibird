#include <queue_message_status.h>
#include <queue_context_provider.h>
#include <pthread.h>
#include <broker_logs_manager.h>
#include <subscriber.h>
#include "publisher.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/pthread_wrapper.h"

void update_subscribers_to_send(t_message_status* message_status, t_queue_context* queue_context){

    pthread_mutex_lock(&(queue_context -> subscribers_mutex));
    list_add_all(message_status -> subscribers_to_send, queue_context -> subscribers);
    pthread_mutex_unlock(&(queue_context -> subscribers_mutex));
}

void join_subscribers_ack_threads(t_list* waiting_for_ack_subscribers_threads, t_queue_context* queue_context){

    for(int i = 0; i < list_size(waiting_for_ack_subscribers_threads); i++){

        t_subscriber_ack_thread* subscriber_ack_thread = (t_subscriber_ack_thread*) list_get(waiting_for_ack_subscribers_threads, i);
        pthread_t waiting_for_ack_thread = subscriber_ack_thread -> subscriber_thread;

        join_reception_for_ack_thread(waiting_for_ack_thread, subscriber_ack_thread -> subscriber_context, subscriber_ack_thread -> message_status, queue_context);
    }
}

void publish(t_message_status* message_status, t_queue_context* queue_context) {

    t_list* subscribers = message_status -> subscribers_to_send;
    t_request* request = create_request_from(message_status);
    t_list* waiting_for_ack_subscribers_threads = list_create();

    if (list_is_empty(subscribers)) {
        log_no_subscribers_for_request(request);
    } else {

        void _send_message(t_subscriber_context* subscriber_context) {
            serialize_and_send_structure(request, subscriber_context -> socket_fd);
            log_succesful_message_sent_to_a_suscriber(request, subscriber_context); //loguea por cada suscriptor al cual se le fue enviado el mensaje.

            pthread_t waiting_for_ack_thread =
                    default_safe_thread_create(receive_ack_thread, (void*) &(subscriber_context -> socket_fd));

            t_subscriber_ack_thread* subscriber_ack_thread = safe_malloc(sizeof(t_subscriber_ack_thread));
            subscriber_ack_thread -> subscriber_thread = waiting_for_ack_thread;
            subscriber_ack_thread -> subscriber_context = subscriber_context;
            subscriber_ack_thread -> message_status = message_status;

            list_add(waiting_for_ack_subscribers_threads, subscriber_ack_thread);

        }
        list_iterate(subscribers, (void (*) (void *)) _send_message);
        log_succesful_message_sent_to_suscribers(request);

        join_subscribers_ack_threads(waiting_for_ack_subscribers_threads, queue_context);
    }
}

void push_to_queue(t_message_status* message_status){

    uint32_t operation = internal_operation_in(message_status -> identified_message);

    if(operation == IDENTIFIED_MESSAGE){ //caso en donde es un identified con otro identified adentro.
        uint32_t internal_operation = internal_operation_in_correlative(message_status->identified_message);
        operation = internal_operation;
    }

    t_queue_context* queue_context = queue_context_with_code(operation);

    update_subscribers_to_send(message_status, queue_context);
    log_succesful_get_and_update_subscribers_to_send(message_status -> identified_message);

    queue_context -> queue_context_operations -> push_message_status_to_queue_function (queue_context, message_status);
    log_succesful_new_message_pushed_to_a_queue(message_status -> identified_message, queue_context -> operation);

    publish(message_status, queue_context);
}