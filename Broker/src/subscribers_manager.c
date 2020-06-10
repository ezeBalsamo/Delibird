#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include <stdint.h>
#include <queue_message_manager.h>
#include <bits/pthreadtypes.h>
#include <broker_logs_manager.h>
#include <sys/socket.h>
#include "subscribers_manager.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/configuration_manager.h"

t_queue_context* get_context_of_a_queue(uint32_t queue){
   char* queue_name = queue_name_of(queue);
   t_dictionary* dictionary =  get_queue_context_dictionary();
   void* queue_context = dictionary_get(dictionary, queue_name);

   return (t_queue_context*) queue_context;
}

void configure_socket_with_timeout(int socket_fd){

    struct timeval tv;
    tv.tv_sec = config_get_int_at("SOCKET_TIMEOUT_FOR_ACK");
    tv.tv_usec = 0;
    setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
}

void move_subscriber_to_ACK(t_message_status* message_status, int subscriber){

    int* subscriber_pointer = &subscriber;
    list_add(message_status -> subscribers_who_received, (void*) subscriber_pointer);
    t_list* subscribers_to_send = message_status -> subscribers_to_send;

    bool equals_subscribers_(void* another_subscriber){
        return subscriber == *((int*) another_subscriber);
    }

    list_remove_by_condition(subscribers_to_send, (bool (*)(void *)) equals_subscribers_);
}

void subscribe_process(int subscriber, uint32_t operation_queue){
    t_queue_context* queue_context = get_context_of_a_queue(operation_queue);
    t_list* subscribers = queue_context -> subscribers;

    int* subscriber_socket_fd = safe_malloc(sizeof(int));
    *subscriber_socket_fd = subscriber;

    configure_socket_with_timeout(*subscriber_socket_fd);
    sem_wait(&(queue_context -> subscribers_mutex));
    list_add(subscribers, (void*) subscriber_socket_fd);
    sem_post(&(queue_context -> subscribers_mutex));
    log_succesful_subscription_process(*subscriber_socket_fd);
}

void disconnect_subscriber(t_message_status* message_status, int subscriber){

    t_list* subscribers_to_send = message_status -> subscribers_to_send;

    bool equals_subscribers_(void* another_subscriber){
        return subscriber == *((int*)another_subscriber);
    }

    list_remove_by_condition(subscribers_to_send, (bool (*)(void *)) equals_subscribers_);
    close(subscriber);
    log_subscriber_disconnection(subscriber);
}

void send_all_messages(int subscriber, uint32_t operation_queue){
    t_queue* queue = get_queue_of(operation_queue);
    void* subscriber_ack;

    for(int i = 0; i < list_size(queue -> elements); i++){

        t_message_status* message_status = list_get(queue -> elements, i);
        t_request* request = create_request_id(message_status);

        serialize_and_send_structure(request, subscriber);

        pthread_t waiting_for_ack_thread = default_safe_thread_create(receive_ack_thread, &subscriber);

        pthread_join(waiting_for_ack_thread, &subscriber_ack);

        uint32_t cast_subscriber_ack = *((uint32_t *) subscriber_ack);

        if (cast_subscriber_ack == FAILED_ACK){
            log_ack_received_error();
            disconnect_subscriber(message_status, subscriber);
        } else {
            move_subscriber_to_ACK(message_status, subscriber);
            log_succesful_all_messages_of_a_queue_sent_to(subscriber);
        }
    }
}