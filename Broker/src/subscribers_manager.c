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

t_list* new_pokemon_subscribers;
t_list* appeared_pokemon_subscribers;
t_list* get_pokemon_subscribers;
t_list* localized_pokemon_subscribers;
t_list* catch_pokemon_subscribers;
t_list* caught_pokemon_subscribers;

t_dictionary* subscribers_list_dictionary;


void initialize_subscribers_list(){
    new_pokemon_subscribers = list_create();
    appeared_pokemon_subscribers = list_create();
    get_pokemon_subscribers = list_create();
    localized_pokemon_subscribers = list_create();
    catch_pokemon_subscribers = list_create();
    caught_pokemon_subscribers = list_create();
}

void initialize_subscribers_list_dictionary(){
    subscribers_list_dictionary = dictionary_create();
    dictionary_put(subscribers_list_dictionary,"NEW_POKEMON", (void*) new_pokemon_subscribers);
    dictionary_put(subscribers_list_dictionary,"APPEARED_POKEMON", (void*) appeared_pokemon_subscribers);
    dictionary_put(subscribers_list_dictionary,"GET_POKEMON", (void*) get_pokemon_subscribers);
    dictionary_put(subscribers_list_dictionary,"LOCALIZED_POKEMON", (void*) localized_pokemon_subscribers);
    dictionary_put(subscribers_list_dictionary,"CATCH_POKEMON", (void*) catch_pokemon_subscribers);
    dictionary_put(subscribers_list_dictionary,"CAUGHT_POKEMON", (void*) caught_pokemon_subscribers);
}

void initialize_subscribers_manager(){
    initialize_subscribers_list();
    initialize_subscribers_list_dictionary();

    log_succesful_initialize_subscriber_manager();
}

t_list* get_subscribers_of_a_queue(uint32_t queue){
   char* queue_name = queue_name_of(queue);
   return dictionary_get(subscribers_list_dictionary, queue_name);
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
        return subscriber == *((int*)another_subscriber);
    }

    list_remove_by_condition(subscribers_to_send, (bool (*)(void *)) equals_subscribers_);
}

void subscribe_process(int subscriber, uint32_t operation_queue){
    char* queue_name = queue_name_of(operation_queue);
    t_list* subscribers = (t_list*) dictionary_get(subscribers_list_dictionary, queue_name);

    int* subscriber_socket_fd = safe_malloc(sizeof(int));
    *subscriber_socket_fd = subscriber;

    configure_socket_with_timeout(*subscriber_socket_fd);
    list_add(subscribers, (void*) subscriber_socket_fd);
    log_succesful_subscription_process(*subscriber_socket_fd);
}

void send_all_messages(int subscriber, uint32_t operation_queue){
    t_queue* queue = get_queue_of(operation_queue);

    for(int i = 0; i < list_size(queue -> elements); i++){
        t_message_status* message_status = list_get(queue -> elements, i);
        t_request* request = create_request_id(message_status);
        serialize_and_send_structure(request, subscriber);

        pthread_t waiting_for_ack_thread = default_safe_thread_create(receive_ack_thread, &subscriber);
        thread_join(waiting_for_ack_thread);

        move_subscriber_to_ACK(message_status, subscriber);
    }
    log_succesful_all_messages_of_a_queue_sent_to(subscriber);
}

void free_subscriber_dictionary(){
    dictionary_destroy_and_destroy_elements(subscribers_list_dictionary,
                                            (void (*)(void *)) list_destroy_and_destroy_elements);
}

void free_subscribers_manager(){
    free_subscriber_dictionary();
}