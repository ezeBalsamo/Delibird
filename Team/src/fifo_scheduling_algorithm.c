#include <commons/string.h>
#include "fifo_scheduling_algorithm.h"
#include "../../Utils/include/common_structures.h"

t_scheduling_algorithm* fifo_algorithm;

bool fifo_can_handle(char* scheduling_algorithm_name){
    return string_equals_ignore_case(scheduling_algorithm_name, "FIFO");
}

void fifo_update_ready_queue_when_adding_function(t_queue* ready_queue, t_trainer_thread_context* trainer_thread_context){
    queue_push(ready_queue, trainer_thread_context);
}

bool fifo_should_execute_now_function(t_trainer_thread_context* trainer_thread_context){
    (void) trainer_thread_context;
    return basic_should_execute();
}

void fifo_execution_cycle_consumed_function(){
    //do nothing
}

void fifo_reset_quantum_consumed_function(){
    //do nothing
}

void initialize_fifo_scheduling_algorithm(){
    fifo_algorithm = safe_malloc(sizeof(t_scheduling_algorithm));
    fifo_algorithm -> can_handle_function = fifo_can_handle;
    fifo_algorithm -> update_ready_queue_when_adding_function = fifo_update_ready_queue_when_adding_function;
    fifo_algorithm -> should_execute_now_function = fifo_should_execute_now_function;
    fifo_algorithm -> execution_cycle_consumed_function = fifo_execution_cycle_consumed_function;
    fifo_algorithm -> reset_quantum_consumed_function = fifo_reset_quantum_consumed_function;
}

t_scheduling_algorithm* fifo_scheduling_algorithm(){
    return fifo_algorithm;
}