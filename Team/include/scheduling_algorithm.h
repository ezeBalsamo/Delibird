#ifndef DELIBIRD_SCHEDULING_ALGORITHM_H
#define DELIBIRD_SCHEDULING_ALGORITHM_H

#include <commons/collections/queue.h>
#include "trainer_threads.h"

typedef struct Scheduling_algorithm{
    char* algorithm_name;
    void (*update_ready_queue_when_adding_function) (t_list* ready_trainer_thread_contexts, t_trainer_thread_context* trainer_thread_context);
    void (*resolve_dependencies_function) ();
}t_scheduling_algorithm;

void initialize_scheduling_algorithm();

void update_ready_queue_when_adding(t_list* ready_trainer_thread_contexts, t_trainer_thread_context* trainer_thread_context);

char* scheduling_algorithm_name();

void free_scheduling_algorithm();

#endif //DELIBIRD_SCHEDULING_ALGORITHM_H
