#include "../include/queue_context_provider.h"
#include "commons/collections/queue.h"
#include "../include/broker_logs_manager.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include "subscriber.h"
#include <stdlib.h>
#include <commons/collections/dictionary.h>
#include <semaphore.h>
#include "../../Utils/include/pthread_wrapper.h"

t_dictionary* queue_context_by_queue_name;

void initialize_and_load_queue_context_for(uint32_t queue_code){

    pthread_mutex_t queue_mutex;
    pthread_mutex_t subscribers_mutex;
    t_queue* queue = queue_create();
    t_list* subscribers = list_create();
    pthread_mutex_init(&queue_mutex, NULL);
    pthread_mutex_init(&subscribers_mutex, NULL);

    t_queue_context* queue_context = safe_malloc(sizeof(t_queue_context));
    uint32_t operation = queue_code;
    queue_context -> operation = operation;
    queue_context -> queue = queue;
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

void free_queue_context(t_queue_context* queue_context){

    queue_destroy_and_destroy_elements((queue_context -> queue), (void (*)(void *)) free_message_status);
    list_destroy_and_destroy_elements(queue_context -> subscribers, free);
    free(queue_context);
}

void free_queue_context_provider(){
    dictionary_destroy_and_destroy_elements(queue_context_by_queue_name, (void (*)(void *)) free_queue_context);
}