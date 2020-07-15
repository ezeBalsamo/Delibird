#include <queue_message_status.h>
#include <queue_context_provider.h>
#include <pthread.h>
#include <broker_logs_manager.h>
#include <subscriber.h>
#include <stdlib.h>
#include "publisher.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/garbage_collector.h"
#include "broker_memory_manager.h"
#include <string.h>

void update_subscribers_to_send(t_message_status* message_status, t_queue_context* queue_context){

    pthread_mutex_lock(&(queue_context -> subscribers_mutex));
    list_add_all(message_status -> subscribers_to_send, queue_context -> subscribers);
    pthread_mutex_unlock(&(queue_context -> subscribers_mutex));
}

void free_subscriber_ack_thread(t_subscriber_ack_thread* subscriber_ack_thread){
    pthread_cancel(subscriber_ack_thread -> subscriber_thread);
    free(subscriber_ack_thread);
}

void join_subscribers_ack_threads(t_list* waiting_for_ack_subscribers_threads, t_queue_context* queue_context){

    for(int i = 0; i < list_size(waiting_for_ack_subscribers_threads); i++){

        t_subscriber_ack_thread* subscriber_ack_thread = (t_subscriber_ack_thread*) list_get(waiting_for_ack_subscribers_threads, i);
        pthread_t waiting_for_ack_thread = subscriber_ack_thread -> subscriber_thread;

        void* ack = join_reception_for_ack_thread(waiting_for_ack_thread, subscriber_ack_thread -> subscriber_context, subscriber_ack_thread -> message_status, queue_context);
        free(ack);
    }
    list_destroy_and_destroy_elements(waiting_for_ack_subscribers_threads,
                                      (void (*)(void *)) free_subscriber_ack_thread);
}

t_serialization_information* create_serialization_information_of_identified_message_with_correlative_id_from(t_memory_block* memory_block){

    uint32_t amount_of_bytes_of_request = memory_block -> message_size + sizeof(uint32_t) + sizeof(uint32_t); //por el tamaño de la estructura y la operacion.

    uint32_t amount_of_bytes_of_identified = amount_of_bytes_of_request + sizeof(uint32_t) * 4; //por la operacion, el tamaño del identified y el message id y el tamaño de la request.

    uint32_t amount_of_bytes_of_identified_with_correlative = amount_of_bytes_of_identified + sizeof(uint32_t) * 4; //por el tamaño del identified y e

    uint32_t amount_of_bytes_of_identified_request = amount_of_bytes_of_identified + sizeof(uint32_t);

    void* serialized_request = malloc(amount_of_bytes_of_identified_with_correlative);

    uint32_t identified_message = IDENTIFIED_MESSAGE;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &identified_message, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_identified_with_correlative, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &memory_block -> message_id , sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_identified_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &identified_message, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_identified, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &memory_block -> correlative_message_id, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &memory_block -> message_operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &memory_block -> message_size, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, memory_block -> message, memory_block -> message_size);

    t_serialization_information* request_serialization_information = malloc(sizeof(t_serialization_information));
    request_serialization_information -> amount_of_bytes = amount_of_bytes_of_identified_with_correlative;
    request_serialization_information -> serialized_request = serialized_request;

    return request_serialization_information;
}

t_serialization_information* create_serialization_information_of_identified_message_from(t_memory_block* memory_block){

    uint32_t amount_of_bytes_of_request = memory_block -> message_size + sizeof(uint32_t) + sizeof(uint32_t); //por el tamaño de la estructura y la operacion.

    uint32_t amount_of_bytes_of_identified = amount_of_bytes_of_request + sizeof(uint32_t) * 4; //por la operacion, el tamaño del identified y el message id y el tamaño de la request.

    void* serialized_request = malloc(amount_of_bytes_of_identified);

    uint32_t identified_message = IDENTIFIED_MESSAGE;

    uint32_t offset = 0;

    memcpy(serialized_request + offset, &identified_message, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_identified, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &memory_block -> message_id, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &amount_of_bytes_of_request, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &memory_block -> message_operation, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, &memory_block -> message_size, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(serialized_request + offset, memory_block -> message, memory_block -> message_size);

    t_serialization_information* request_serialization_information = malloc(sizeof(t_serialization_information));
    request_serialization_information -> amount_of_bytes = amount_of_bytes_of_identified;
    request_serialization_information -> serialized_request = serialized_request;

    return request_serialization_information;
}

t_serialization_information* create_serialization_information_from(t_memory_block* memory_block){

    if(memory_block -> correlative_message_id != 0){
        return create_serialization_information_of_identified_message_with_correlative_id_from(memory_block);
    }

    return create_serialization_information_of_identified_message_from(memory_block);
}

void publish(t_message_status* message_status, t_queue_context* queue_context) {

    t_list* subscribers = message_status -> subscribers_to_send;
    t_memory_block* memory_block = get_memory_block_from_memory(message_status -> message_id);

    t_serialization_information* serialization_information = create_serialization_information_from(memory_block);
    t_list* waiting_for_ack_subscribers_threads = list_create();

    if (list_is_empty(subscribers)) {
        log_no_subscribers_for_message_with_id(message_status -> message_id);
        list_destroy(waiting_for_ack_subscribers_threads);
    } else {

            void _send_message(t_subscriber_context* subscriber_context) {

                send_serialized_structure(serialization_information, subscriber_context -> socket_fd);
                log_succesful_message_sent_to_a_suscriber(message_status -> message_id, subscriber_context); //loguea por cada suscriptor al cual se le fue enviado el mensaje.

                pthread_t waiting_for_ack_thread =
                        default_safe_thread_create(receive_ack_thread, (void*) &(subscriber_context -> socket_fd));

                t_subscriber_ack_thread* subscriber_ack_thread = safe_malloc(sizeof(t_subscriber_ack_thread));
                subscriber_ack_thread -> subscriber_thread = waiting_for_ack_thread;
                subscriber_ack_thread -> subscriber_context = subscriber_context;
                subscriber_ack_thread -> message_status = message_status;

                list_add(waiting_for_ack_subscribers_threads, subscriber_ack_thread);
                free_serialization_information(serialization_information);
            }

            t_list* subscribers_with_connection_active = list_filter(subscribers, (bool (*)(void *)) has_active_connection);
            list_iterate(subscribers_with_connection_active, (void (*) (void *)) _send_message);
            list_destroy(subscribers_with_connection_active);
            log_succesful_message_sent_to_suscribers(message_status -> message_id);

            join_subscribers_ack_threads(waiting_for_ack_subscribers_threads, queue_context);
        }
}

void push_to_queue(t_message_status* message_status){

    uint32_t operation = message_status -> operation_queue;

    t_queue_context* queue_context = queue_context_with_code(operation);

    update_subscribers_to_send(message_status, queue_context);
    log_succesful_get_and_update_subscribers_to_send(message_status -> message_id);

    queue_context -> queue_context_operations -> add_message_status_to_queue_function (queue_context, message_status);
    log_succesful_new_message_pushed_to_a_queue(message_status -> message_id, queue_context -> operation);

    publish(message_status, queue_context);
}