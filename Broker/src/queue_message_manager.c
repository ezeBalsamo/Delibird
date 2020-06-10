#include "../include/queue_message_manager.h"
#include "commons/collections/queue.h"
#include "../../Utils/include/socket.h"
#include "../include/broker_logs_manager.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include "subscribers_manager.h"
#include <stdlib.h>
#include <free_broker.h>
#include <commons/collections/dictionary.h>
#include <publish_message_mode.h>
#include <semaphore.h>
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/t_list_extension.h"

t_dictionary* queue_management_dictionary;

t_dictionary* get_queue_context_dictionary(){
    return queue_management_dictionary;
}

void initialize_queue_context(char* operation_name){

    t_queue* queue = queue_create();
    t_list* subscribers = list_create();
    sem_t queue_mutex;
    sem_init(&queue_mutex, false, 1);
    sem_t subscribers_mutex;
    sem_init(&subscribers_mutex, false, 1);

    t_queue_context* queue_context = safe_malloc(sizeof(t_queue_context));
    uint32_t operation = queue_code_of(operation_name);
    queue_context -> operation = operation;
    queue_context -> queue = queue;
    queue_context -> subscribers = subscribers;
    queue_context -> queue_mutex = queue_mutex;
    queue_context -> subscribers_mutex = subscribers_mutex;

    dictionary_put(queue_management_dictionary, operation_name, (void*) queue_context);
}

void initialize_queue_dictionary(){

    queue_management_dictionary = dictionary_create();
    initialize_queue_context("NEW_POKEMON");
    initialize_queue_context("APPEARED_POKEMON");
    initialize_queue_context("GET_POKEMON");
    initialize_queue_context("LOCALIZED_POKEMON");
    initialize_queue_context("CATCH_POKEMON");
    initialize_queue_context("CAUGHT_POKEMON");
}

void initialize_queue_message_manager(){

    initialize_queue_dictionary();
    log_succesful_initialize_queue_message_manager();
}

t_request* create_request_id(t_message_status* message_status){

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = IDENTIFIED_MESSAGE;
    request -> structure = message_status -> identified_message;
    request -> sanitizer_function = (void (*)(void *)) free_identified_message;

    return request;
}

void get_and_update_subscribers_to_send(t_message_status* message_status, t_queue_context* queue_context){

    sem_wait(&(queue_context -> subscribers_mutex));
    list_add_all(message_status -> subscribers_to_send, queue_context -> subscribers);
    sem_post(&(queue_context -> subscribers_mutex));
}

t_queue* get_queue_of(uint32_t operation){

    char* queue_name = queue_name_of(operation);
    void* queue_management_structure = dictionary_get(queue_management_dictionary, queue_name);

    return ((t_queue_context*) queue_management_structure) -> queue;
}

bool equals_message_status_(t_message_status* message_status, t_message_status* another_message_status){
    return message_status -> identified_message == another_message_status -> identified_message &&
            are_equal_lists(message_status->subscribers_to_send, another_message_status->subscribers_to_send) &&
           are_equal_lists(message_status->subscribers_who_received, another_message_status->subscribers_who_received);
}

void push_to_queue(t_message_status* message_status){

    uint32_t operation = internal_operation_in(message_status -> identified_message);

    if(operation == IDENTIFIED_MESSAGE){ //caso en donde es un identified con otro identified adentro.
        uint32_t internal_operation = internal_operation_in_correlative(message_status->identified_message);
        operation = internal_operation;
    }

    t_queue_context* queue_context = get_context_of_a_queue(operation);

    get_and_update_subscribers_to_send(message_status, queue_context);
    log_succesful_get_and_update_subscribers_to_send(message_status -> identified_message);

    sem_wait(&(queue_context -> queue_mutex));
    queue_push(queue_context -> queue, message_status);
    sem_post(&(queue_context -> queue_mutex));
    log_succesful_new_message_pushed_to_a_queue(message_status -> identified_message);

    publish(message_status);

    //TODO ESTO ES LA LOGICA DE BORRAR MENSAJE EN LA COLA, VER MAS ADELANTE EN QUE MOMENTO SE DEBERIA DE HACER ESTO.
    //Ahora se esta haciendo cuando el message_status ya no tiene suscriptores a los cuales enviarle.
/*    if(list_is_empty(message_status -> subscribers_to_send)){ //si el mensaje fue enviado a todos los suscriptores se borra.
        for(int i = 0; i < list_size(queue -> elements); i++){
            t_message_status* message_status_to_compare = (t_message_status*) list_get(queue->elements, i);
            if(equals_message_status_(message_status, message_status_to_compare)){
                list_remove(queue -> elements, i);
                free_message_status(message_status);
            }
        }
    }*/
}

void join_subscribers_threads(t_list* subscriber_thread_list){
    void* subscriber_ack;

    for(int i = 0; i < list_size(subscriber_thread_list); i++){

        t_subscriber_ack_thread* subscriber_ack_thread = (t_subscriber_ack_thread*) list_get(subscriber_thread_list, i);
        pthread_t waiting_for_ack_thread = subscriber_ack_thread -> subscriber_thread;

        pthread_join(waiting_for_ack_thread, &subscriber_ack);

        uint32_t cast_subscriber_ack = *((uint32_t *) subscriber_ack);

        if (cast_subscriber_ack == FAILED_ACK){
            log_ack_received_error();
            disconnect_subscriber(subscriber_ack_thread->message_status, subscriber_ack_thread->subscriber);
        } else {
            move_subscriber_to_ACK(subscriber_ack_thread -> message_status, subscriber_ack_thread -> subscriber);
        }
    }
}

void publish(t_message_status* message_status) {

    t_list *subscribers = message_status -> subscribers_to_send;
    t_request *request = create_request_id(message_status);
    t_list* subscriber_thread_list = list_create();

    if (list_is_empty(subscribers)) {
        log_no_subscribers_for_request(request);
    } else {

        void _send_message(void* subscriber) {
            int cast_subscriber = *((int *) subscriber);
            serialize_and_send_structure(request, cast_subscriber);
            log_succesful_message_sent_to_a_suscriber(request); //loguea por cada suscriptor al cual se el fue enviado el mensaje.


            pthread_t waiting_for_ack_thread = default_safe_thread_create(receive_ack_thread, subscriber);

            t_subscriber_ack_thread* subscriber_ack_thread = malloc(sizeof(t_subscriber_ack_thread));
            subscriber_ack_thread -> subscriber_thread = waiting_for_ack_thread;
            subscriber_ack_thread -> subscriber = cast_subscriber;
            subscriber_ack_thread -> message_status = message_status;

            list_add(subscriber_thread_list, subscriber_ack_thread);

        }
        list_iterate(subscribers, _send_message);
        log_succesful_message_sent_to_suscribers(request);

        join_subscribers_threads(subscriber_thread_list);
        }
}

void free_queue_context(t_queue_context* queue_context){

    queue_destroy_and_destroy_elements((queue_context -> queue), (void (*)(void *)) free_message_status);
    list_destroy(queue_context -> subscribers);
}

void free_queue_dictionary(){
    dictionary_destroy_and_destroy_elements(queue_management_dictionary, (void (*)(void *)) free_queue_context);
}

void free_queue_message_manager(){
    free_queue_dictionary();
}