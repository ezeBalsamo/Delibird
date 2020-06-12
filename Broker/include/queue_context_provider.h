#ifndef TP_2020_1C_EL_KUELGUE_QUEUE_MESSAGE_MANAGER_H
#define TP_2020_1C_EL_KUELGUE_QUEUE_MESSAGE_MANAGER_H

#include <commons/collections/queue.h>
#include <stdint.h>
#include <zconf.h>

typedef struct Queue_context_operations{
    void (*add_subscriber_function) (void*, void*);
    void (*remove_subscriber_function) (void*, void*);
    void (*push_message_status_to_queue_function) (void*, void*);
    void (*remove_message_status_of_queue_function) (void*, void*);
}t_queue_context_operations;

typedef struct Queue_context{
    uint32_t operation;
    t_queue* queue;
    pthread_mutex_t queue_mutex;
    t_list* subscribers;
    pthread_mutex_t subscribers_mutex;
    t_queue_context_operations* queue_context_operations;
} t_queue_context;

void initialize_queue_context_provider();

t_queue_context* queue_context_of_queue_named(char* queue_name);
t_queue_context* queue_context_with_code(uint32_t queue);

void free_queue_context_provider();

#endif //TP_2020_1C_EL_KUELGUE_QUEUE_MESSAGE_MANAGER_H
