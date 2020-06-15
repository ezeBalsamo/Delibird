#include "../include/queue_context_provider.h"
#include "../include/broker_logs_manager.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include "subscriber.h"
#include <stdlib.h>
#include <commons/collections/dictionary.h>
#include <queue_context_operations.h>
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/socket.h"
#include <commons/string.h>

t_dictionary* queue_context_by_queue_name;

void initialize_and_load_queue_context_for(uint32_t queue_code){

    pthread_mutex_t queue_mutex;
    pthread_mutex_t subscribers_mutex;
    t_list* messages = list_create();
    t_list* subscribers = list_create();
    pthread_mutex_init(&queue_mutex, NULL);
    pthread_mutex_init(&subscribers_mutex, NULL);

    t_queue_context* queue_context = safe_malloc(sizeof(t_queue_context));
    uint32_t operation = queue_code;
    queue_context -> operation = operation;
    queue_context -> messages = messages;
    queue_context -> subscribers = subscribers;
    queue_context -> queue_mutex = queue_mutex;
    queue_context -> subscribers_mutex = subscribers_mutex;
    queue_context -> queue_context_operations = new_queue_context_operations();

    dictionary_put(queue_context_by_queue_name, queue_name_of(queue_code), (void*) queue_context);
}

void initialize_queue_context_by_name(){

    queue_context_by_queue_name = dictionary_create();
    initialize_and_load_queue_context_for(NEW_POKEMON);
    initialize_and_load_queue_context_for(APPEARED_POKEMON);
    initialize_and_load_queue_context_for(GET_POKEMON);
    initialize_and_load_queue_context_for(LOCALIZED_POKEMON);
    initialize_and_load_queue_context_for(CATCH_POKEMON);
    initialize_and_load_queue_context_for(CAUGHT_POKEMON);
}

void initialize_queue_context_provider(){

    initialize_queue_context_by_name();
    log_succesful_initialize_queue_context_provider();
}

t_queue_context* queue_context_of_queue_named(char* queue_name){
    return dictionary_get(queue_context_by_queue_name, queue_name);
}

t_queue_context* queue_context_with_code(uint32_t queue){
    char* queue_name = queue_name_of(queue);
    return queue_context_of_queue_named(queue_name);
}

t_subscriber_context* old_suscriptor_of(t_queue_context* queue_context, t_subscriber_context* subscriber_to_find){

    bool _was_subscribed(t_subscriber_context* subscriber_to_compare){
        return are_equivalent_subscribers(subscriber_to_find, subscriber_to_compare);
    }

    return list_remove_by_condition(queue_context -> subscribers, (bool (*)(void *)) _was_subscribed);
}

void free_and_close_subscriber_connection(t_subscriber_context* subscriber){
    close_connection(subscriber -> socket_fd);
    free(subscriber);
}

void free_queue_context(t_queue_context* queue_context){

    list_destroy_and_destroy_elements(queue_context -> messages, (void (*)(void *)) free_message_status);
    list_iterate(queue_context->subscribers, (void (*)(void *)) free_and_close_subscriber_connection);
    list_destroy(queue_context -> subscribers);
    free(queue_context -> queue_context_operations);
    free(queue_context);
}

void free_queue_context_provider(){
    dictionary_destroy_and_destroy_elements(queue_context_by_queue_name, (void (*)(void *)) free_queue_context);
}