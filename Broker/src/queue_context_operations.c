#include "../../Utils/include/common_structures.h"
#include <queue_context_provider.h>
#include <pthread.h>
#include <queue_message_status.h>

void queue_context_add_subscriber_function(t_queue_context* queue_context, t_subscriber_context* subscriber_context){
    pthread_mutex_lock(&queue_context -> subscribers_mutex);
    list_add(queue_context -> subscribers, subscriber_context);
    pthread_mutex_unlock(&queue_context -> subscribers_mutex);
}

void queue_context_push_message_status_to_queue_function(t_queue_context* queue_context, t_message_status* message_status){
    pthread_mutex_lock(&(queue_context -> queue_mutex));
    list_add(queue_context -> messages, message_status);
    pthread_mutex_unlock(&(queue_context -> queue_mutex));
}

t_queue_context_operations* new_queue_context_operations(){
    t_queue_context_operations* queue_context_operations = safe_malloc(sizeof(t_queue_context_operations));
    queue_context_operations -> add_subscriber_function = (void (*)(void *, void *)) queue_context_add_subscriber_function;
    queue_context_operations -> add_message_status_to_queue_function =
            (void (*)(void *, void *)) queue_context_push_message_status_to_queue_function;
    return queue_context_operations;
}