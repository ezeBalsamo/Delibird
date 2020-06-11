#include "../include/queue_context_provider.h"
#include "commons/collections/queue.h"
#include "../include/broker_logs_manager.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include "subscriber.h"
#include <stdlib.h>
#include <commons/collections/dictionary.h>
#include <semaphore.h>
#include "../../Utils/include/pthread_wrapper.h"

t_dictionary* queue_contexts_by_queue_name;

t_queue_context* get_queue_context_of_queue_named(char* queue_name){
    void* queue_context = dictionary_get(queue_contexts_by_queue_name, queue_name);
    return (t_queue_context*) queue_context;
}

t_queue_context* get_context_of_operation_queue(uint32_t queue){
    char* queue_name = queue_name_of(queue);
    return get_queue_context_of_queue_named(queue_name);
}

void initialize_and_load_queue_context_for(uint32_t queue_code){

    pthread_mutex_t queue_mutex;
    pthread_mutex_t subscribers_mutex;
    t_queue* queue = queue_create();
    t_list* subscribers = list_create();
    pthread_mutex_init(&queue_mutex, NULL);
    pthread_mutex_init(&subscribers_mutex, NULL);

    t_queue_context* queue_context = safe_malloc(sizeof(t_queue_context));
    queue_context -> operation = queue_code;
    queue_context -> queue = queue;
    queue_context -> subscribers = subscribers;
    queue_context -> queue_mutex = queue_mutex;
    queue_context -> subscribers_mutex = subscribers_mutex;

    dictionary_put(queue_contexts_by_queue_name, queue_name_of(queue_code), (void*) queue_context);
}

void initialize_queue_context_dictionary(){

    queue_contexts_by_queue_name = dictionary_create();
    initialize_and_load_queue_context_for(NEW_POKEMON);
    initialize_and_load_queue_context_for(APPEARED_POKEMON);
    initialize_and_load_queue_context_for(GET_POKEMON);
    initialize_and_load_queue_context_for(LOCALIZED_POKEMON);
    initialize_and_load_queue_context_for(CATCH_POKEMON);
    initialize_and_load_queue_context_for(CAUGHT_POKEMON);
}

void initialize_queue_context_provider(){

    initialize_queue_context_dictionary();
    log_succesful_initialize_queue_message_manager();
}

t_request* create_request_from(t_message_status* message_status){

    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = IDENTIFIED_MESSAGE;
    request -> structure = message_status -> identified_message;
    request -> sanitizer_function = (void (*)(void *)) free_identified_message;

    return request;
}

void free_queue_context(t_queue_context* queue_context){

    queue_destroy_and_destroy_elements((queue_context -> queue), (void (*)(void *)) free_message_status);
    list_destroy_and_destroy_elements(queue_context -> subscribers, free);
    free(queue_context);
}

void free_queue_context_provider(){
    dictionary_destroy_and_destroy_elements(queue_contexts_by_queue_name, (void (*)(void *)) free_queue_context);
}