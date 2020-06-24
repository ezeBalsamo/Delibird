#ifndef DELIBIRD_DISPATCHER_QUEUES_H
#define DELIBIRD_DISPATCHER_QUEUES_H

#include <stdint.h>
#include <pthread.h>
#include <commons/collections/queue.h>
#include "trainer_threads.h"

typedef struct Dispatcher_queue{
    uint32_t state;
    pthread_mutex_t* mutex;
    t_list* trainer_thread_contexts;
}t_dispatcher_queue;

void initialize_dispatcher_queues();

t_dispatcher_queue* dispatcher_queue_of(uint32_t state);
t_list* trainer_thread_contexts_in(uint32_t state);

void move_to(t_trainer_thread_context* trainer_thread_context, uint32_t to_queue);
void add_to_dispatcher_queue(t_trainer_thread_context* trainer_thread_context, uint32_t state);
void remove_from_dispatcher_queue(t_trainer_thread_context* trainer_thread_context);
void remove_from_execute();

void free_dispatcher_queues();

#endif //DELIBIRD_DISPATCHER_QUEUES_H
