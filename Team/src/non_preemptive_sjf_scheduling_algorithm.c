#include "non_preemptive_sjf_scheduling_algorithm.h"
#include <commons/string.h>
#include "../../Utils/include/common_structures.h"

t_scheduling_algorithm* non_preemptive_sjf_algorithm;

bool non_preemptive_sjf_scheduling_algorithm_can_handle(char* scheduling_algorithm_name){
    return string_equals_ignore_case(scheduling_algorithm_name, "SJF-SD");
}

void non_preemptive_update_ready_queue_when_adding_function(t_queue* ready_queue, t_trainer_thread_context* trainer_thread_context){
    //TODO lógica en la que me va ayudar el groso de nico
}

bool non_preemptive_sjf_should_execute_now_function(t_trainer_thread_context* trainer_thread_context){
    (void) trainer_thread_context;
    return basic_should_execute();
}

void non_preemptive_sjf_execution_cycle_consumed_function(){
    //do nothing
}


void non_preemptive_sjf_reset_quantum_consumed_function(){
    //do nothing
}

void initialize_non_preemptive_sjf_scheduling_algorithm(){
    non_preemptive_sjf_algorithm = safe_malloc(sizeof(t_scheduling_algorithm));
    non_preemptive_sjf_algorithm -> can_handle_function = non_preemptive_sjf_scheduling_algorithm_can_handle;
    non_preemptive_sjf_algorithm -> update_ready_queue_when_adding_function = non_preemptive_update_ready_queue_when_adding_function;
    non_preemptive_sjf_algorithm -> should_execute_now_function = non_preemptive_sjf_should_execute_now_function;
    non_preemptive_sjf_algorithm -> execution_cycle_consumed_function = non_preemptive_sjf_execution_cycle_consumed_function;
    non_preemptive_sjf_algorithm -> reset_quantum_consumed_function = non_preemptive_sjf_reset_quantum_consumed_function;
}

t_scheduling_algorithm* non_preemptive_sjf_scheduling_algorithm(){
    return non_preemptive_sjf_algorithm;
}