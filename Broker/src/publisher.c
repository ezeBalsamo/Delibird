#include <queue_message_status.h>
#include <queue_context_provider.h>
#include <pthread.h>
#include <broker_logs_manager.h>
#include <subscriber.h>
#include "publisher.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/socket.h"

void update_subscribers_to_send(t_message_status* message_status, t_queue_context* queue_context){

    pthread_mutex_lock(&(queue_context -> subscribers_mutex));
    list_add_all(message_status -> subscribers_to_send, queue_context -> subscribers);
    pthread_mutex_unlock(&(queue_context -> subscribers_mutex));
}

bool are_equals_message_status(t_message_status* message_status, t_message_status* another_message_status){
    return message_status -> identified_message == another_message_status -> identified_message &&
           are_equals_lists(message_status->subscribers_to_send, another_message_status->subscribers_to_send) &&
           are_equals_lists(message_status->subscribers_who_received, another_message_status->subscribers_who_received);
}

void join_subscribers_ack_threads(t_list* subscriber_thread_list){
    void* subscriber_ack;

    for(int i = 0; i < list_size(subscriber_thread_list); i++){

        t_subscriber_ack_thread* subscriber_ack_thread = (t_subscriber_ack_thread*) list_get(subscriber_thread_list, i);
        pthread_t waiting_for_ack_thread = subscriber_ack_thread -> subscriber_thread;

        pthread_join(waiting_for_ack_thread, &subscriber_ack);

        uint32_t cast_subscriber_ack = *((uint32_t *) subscriber_ack);

        if (cast_subscriber_ack == FAILED_ACK){
            log_ack_failed_to_received_error();
        //    revoke_suscription(subscriber_ack_thread->message_status,  subscriber_ack_thread->subscriber);
        } else {
            move_subscriber_to_ACK(subscriber_ack_thread -> message_status, subscriber_ack_thread -> subscriber);
        }
    }
}

void publish(t_message_status* message_status) {

    t_list *subscribers = message_status -> subscribers_to_send;
    t_request *request = create_request_from(message_status);
    t_list* waiting_for_ack_subscribers_threads = list_create();

    if (list_is_empty(subscribers)) {
        log_no_subscribers_for_request(request);
    } else {

        void _send_message(void* subscriber) {
            int cast_subscriber = *((int *) subscriber);
            serialize_and_send_structure(request, cast_subscriber);
            log_succesful_message_sent_to_a_suscriber(request); //loguea por cada suscriptor al cual se el fue enviado el mensaje.


            pthread_t waiting_for_ack_thread = default_safe_thread_create(receive_ack_thread, subscriber);

            t_subscriber_ack_thread* subscriber_ack_thread = safe_malloc(sizeof(t_subscriber_ack_thread));
            subscriber_ack_thread -> subscriber_thread = waiting_for_ack_thread;
            subscriber_ack_thread -> subscriber = cast_subscriber;
            subscriber_ack_thread -> message_status = message_status;

            list_add(waiting_for_ack_subscribers_threads, subscriber_ack_thread);

        }
        list_iterate(subscribers, _send_message);
        log_succesful_message_sent_to_suscribers(request);

        join_subscribers_ack_threads(waiting_for_ack_subscribers_threads);
    }
}

void push_to_queue(t_message_status* message_status){

    uint32_t operation = internal_operation_in(message_status -> identified_message);

    if(operation == IDENTIFIED_MESSAGE){ //caso en donde es un identified con otro identified adentro.
        uint32_t internal_operation = internal_operation_in_correlative(message_status->identified_message);
        operation = internal_operation;
    }

    t_queue_context* queue_context = get_context_of_operation_queue(operation);

    update_subscribers_to_send(message_status, queue_context);
    log_succesful_get_and_update_subscribers_to_send(message_status -> identified_message);

    pthread_mutex_lock(&(queue_context -> queue_mutex));
    queue_push(queue_context -> queue, message_status);
    pthread_mutex_unlock(&(queue_context -> queue_mutex));
    log_succesful_new_message_pushed_to_a_queue(message_status -> identified_message);

    publish(message_status);

    //TODO ESTO ES LA LOGICA DE BORRAR MENSAJE EN LA COLA, VER MAS ADELANTE EN QUE MOMENTO SE DEBERIA DE HACER ESTO.
    //Ahora se esta haciendo cuando el message_status ya no tiene suscriptores a los cuales enviarle.
/*    if(list_is_empty(message_status -> subscribers_to_send)){ //si el mensaje fue enviado a todos los suscriptores se borra.
        for(int i = 0; i < list_size(queue -> elements); i++){
            t_message_status* message_status_to_compare = (t_message_status*) list_get(queue->elements, i);
            if(are_equals_message_status(message_status, message_status_to_compare)){
                list_remove(queue -> elements, i);
                free_message_status(message_status);
            }
        }
    }*/
}