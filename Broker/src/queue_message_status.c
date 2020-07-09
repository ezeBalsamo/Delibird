#include <stdlib.h>
#include <pthread.h>
#include <broker_logs_manager.h>
#include <subscriber.h>
#include "../include/queue_message_status.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/queue_code_name_associations.h"

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

void delete_message(uint32_t operation_message, uint32_t message_id){

    t_queue_context* queue_context = queue_context_of_queue_named(queue_name_of(operation_message));

    bool _message_status_with_message_id(t_message_status* message_status){
        return message_status -> identified_message -> message_id  == message_id;
    }

    t_message_status* message_status = list_remove_by_condition(queue_context -> messages, (bool (*) (void*)) _message_status_with_message_id);
    free_message_status(message_status);
}

void delete_message_if_necessary(t_message_status* message_status,t_queue_context* queue_context){
    t_list* subscribers_message_who_received = message_status -> subscribers_who_received;
    t_list* subscribers_queue = queue_context -> subscribers;
    bool are_equals_subscribers = true;

    if(list_size(subscribers_message_who_received) == list_size(subscribers_queue)){
        for(int i = 0; i < list_size(subscribers_message_who_received) && are_equals_subscribers; i++){
        bool have_the_same_subscriber =  list_contains(subscribers_queue, list_get(subscribers_message_who_received,i),
                          (bool (*)(void *, void *)) are_equivalent_subscribers);
        are_equals_subscribers = are_equals_subscribers && have_the_same_subscriber;
        }
    }

    if(are_equals_subscribers)
    delete_message(message_status ->identified_message -> request -> operation, message_status -> identified_message -> message_id);
}

void* join_reception_for_ack_thread(pthread_t waiting_for_ack_thread, t_subscriber_context* subscriber_context,
        t_message_status* message_status, t_queue_context* queue_context){

    void *subscriber_ack;
    uint32_t expected_ack = message_status -> identified_message -> message_id;

    pthread_join(waiting_for_ack_thread, &subscriber_ack);

    uint32_t cast_subscriber_ack = *((uint32_t *) subscriber_ack);

    if (cast_subscriber_ack == FAILED_ACK || cast_subscriber_ack != expected_ack){
        log_failed_to_receive_ack_error(subscriber_context);
        set_inactive_connection_for(subscriber_context);

    } else {
        subscriber_context -> last_message_id_received = expected_ack;
        move_subscriber_to_ACK(message_status, subscriber_context);
        //todo ojo con este log, ver si en este momento llega a enviarle todos los mensajes, o se loguea por cada vez que se le envia un msj.
        log_succesful_all_messages_of_a_queue_sent_to(subscriber_context);
        delete_message_if_necessary(message_status, queue_context);
    }

    return subscriber_ack;
}

void* receive_ack_thread(void* subscriber_fd){
    int cast_subscriber_fd = *((int *) subscriber_fd);
    int timeout_in_seconds = config_get_int_at("ACK_TIMEOUT");

    return receive_ack_with_timeout_in_seconds(cast_subscriber_fd, timeout_in_seconds);
}

void move_subscriber_to_ACK(t_message_status* message_status, t_subscriber_context* subscriber_context){

    bool _are_equivalents_subscribers(t_subscriber_context* subscriber_to_compare){
        return are_equivalent_subscribers(subscriber_context, subscriber_to_compare);
    }

    void* subscriber_found =
            list_remove_by_condition(message_status -> subscribers_to_send, (bool (*)(void *)) _are_equivalents_subscribers);

    if(!subscriber_found){
        log_subscriber_not_found_in_message_status_subscribers_error(subscriber_context, message_status -> identified_message);
        free_system();
    }

    list_add(message_status -> subscribers_who_received, (void*) subscriber_found);
}

void free_message_status(t_message_status* message_status){
    list_destroy(message_status -> subscribers_who_received);
    list_destroy(message_status -> subscribers_to_send);
    free(message_status);
}