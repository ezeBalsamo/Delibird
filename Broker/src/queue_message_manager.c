#include "../include/queue_message_manager.h"
#include "commons/collections/queue.h"
#include "../include/broker_deserialization.h"
#include "../../Utils/include/socket.h"
#include "../include/broker_logs_manager.h"
#include <stdlib.h>
#include <free_broker.h>

//son queues con estructuras del tipo: connection_request.

t_queue* appeared_queue;
t_queue* new_queue;
t_queue* catch_queue;
t_queue* caught_queue;
t_queue* get_queue;
t_queue* localized_queue;

t_list* subscribers;

void initialize_messages_queue(){
    appeared_queue = queue_create();
    new_queue = queue_create();
    catch_queue = queue_create();
    caught_queue = queue_create();
    get_queue = queue_create();
    localized_queue = queue_create();
}

void initialize_queue_message_manager(){
    initialize_messages_queue();
    log_succesful_queues_creation();

    subscribers = list_create();
    log_succesful_subscribers_list_creation();
    log_succesful_initialize_queue_message_manager();
}

void subscribe_process(t_connection_request* connection_request){

    t_request* deserialized_request = deserialize(connection_request -> serialized_request);
    t_subscribe_me* suscribe_me = (t_subscribe_me*) deserialized_request -> structure;

    t_subscriber* subscriber = malloc(sizeof(t_subscriber));
    subscriber -> queue = suscribe_me -> operation_queue;
    subscriber -> socket_fd = connection_request -> socket_fd;

    list_add(subscribers, (void*) subscriber);

    free_request(deserialized_request);
}

void push_to_queue(uint32_t operation, t_connection_request* connection_request) {

    switch (operation) {

        case APPEARED_POKEMON :
            queue_push(appeared_queue, connection_request);
            log_succesful_new_message_pushed_to_a_queue(connection_request -> serialized_request);
            break;

        case NEW_POKEMON :
            queue_push(new_queue, connection_request);
            log_succesful_new_message_pushed_to_a_queue(connection_request -> serialized_request);
            break;

        case CATCH_POKEMON :
            queue_push(catch_queue, connection_request);
            log_succesful_new_message_pushed_to_a_queue(connection_request -> serialized_request);
            break;

        case CAUGHT_POKEMON :
            queue_push(caught_queue, connection_request);
            log_succesful_new_message_pushed_to_a_queue(connection_request -> serialized_request);
            break;

        case GET_POKEMON :
            queue_push(get_queue, connection_request);
            log_succesful_new_message_pushed_to_a_queue(connection_request -> serialized_request);
            break;

        case LOCALIZED_POKEMON :
            queue_push(localized_queue, connection_request);
            log_succesful_new_message_pushed_to_a_queue(connection_request -> serialized_request);
            break;

        case SUBSCRIBE_ME :
            subscribe_process(connection_request);
            log_succesful_subscription_process();
            break;

        default:
        received_unknown_operation_error();
    }
}

void publish(uint32_t queue, t_serialization_information* serialization_information){

    bool _is_subscribed(void* subscriber){
        return ((t_subscriber*) subscriber) -> queue == queue;
    }

    t_list* subscribers_of_a_queue = list_filter(subscribers, _is_subscribed);

    void _send_message(void* subscriber){
        send_structure(serialization_information, ((t_subscriber*) subscriber) -> socket_fd);
        log_succesful_message_sent_to_a_suscriber(serialization_information -> serialized_request); //loguea por cada suscriptor al cual se el fue enviado el mensaje.
    }

    if(list_is_empty(subscribers_of_a_queue)){
        log_no_subscribers_for_request(serialization_information -> serialized_request);
    } else {
        list_iterate(subscribers_of_a_queue, _send_message);
        log_succesful_message_sent_to_suscribers(serialization_information -> serialized_request);
    }
    list_destroy(subscribers_of_a_queue);
}

void free_subscriber(t_subscriber* subscriber){
    free(&(subscriber -> queue));
    free(subscriber);
}

void free_all_queues(){
    queue_destroy_and_destroy_elements(appeared_queue, (void (*)(void *)) free_connection_request);
    queue_destroy_and_destroy_elements(new_queue, (void (*)(void *)) free_connection_request);
    queue_destroy_and_destroy_elements(catch_queue, (void (*)(void *)) free_connection_request);
    queue_destroy_and_destroy_elements(caught_queue, (void (*)(void *)) free_connection_request);
    queue_destroy_and_destroy_elements(get_queue, (void (*)(void *)) free_connection_request);
    queue_destroy_and_destroy_elements(localized_queue, (void (*)(void *)) free_connection_request);
}

void free_subscribers_list(){
    list_destroy_and_destroy_elements(subscribers, (void (*)(void *)) free_subscriber);
}

void free_queue_message_manager(){
    free_all_queues();
    free_subscribers_list();
}