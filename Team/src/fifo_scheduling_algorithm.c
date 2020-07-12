#include "fifo_scheduling_algorithm.h"

t_scheduling_algorithm* fifo_algorithm;

void fifo_update_ready_queue_when_adding_function(t_list* ready_trainer_thread_contexts, t_trainer_thread_context* trainer_thread_context){
    list_add(ready_trainer_thread_contexts, trainer_thread_context);
}

void fifo_resolve_dependencies_function(){}

void initialize_fifo_scheduling_algorithm(){
    fifo_algorithm = safe_malloc(sizeof(t_scheduling_algorithm));
    fifo_algorithm -> algorithm_name = "FIFO";
    fifo_algorithm -> update_ready_queue_when_adding_function = fifo_update_ready_queue_when_adding_function;
    fifo_algorithm -> resolve_dependencies_function = fifo_resolve_dependencies_function;
}

t_scheduling_algorithm* fifo_scheduling_algorithm(){
    return fifo_algorithm;
}