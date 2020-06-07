#ifndef DELIBIRD_SCHEDULING_ALGORITHM_H
#define DELIBIRD_SCHEDULING_ALGORITHM_H

#include <commons/collections/queue.h>
#include "trainer_threads.h"

typedef struct Scheduling_algorithm{
    bool (*can_handle_function) (char* scheduling_algorithm_name);
    void (*update_ready_queue_when_adding_function) (t_queue* ready_queue, t_trainer_thread_context* trainer_thread_context);
    bool (*should_execute_now_function) (t_trainer_thread_context* trainer_thread_context);
    void (*execution_cycle_consumed_function) ();
}t_scheduling_algorithm;

void initialize_scheduling_algorithm();

void update_ready_queue_when_adding(t_queue* ready_queue, t_trainer_thread_context* trainer_thread_context);

void trainer_thread_context_ready(t_trainer_thread_context* trainer_thread_context);

bool basic_should_execute();

void execution_cycle_consumed();

#endif //DELIBIRD_SCHEDULING_ALGORITHM_H
