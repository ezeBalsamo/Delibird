#include <commons/string.h>
#include <dispatcher.h>
#include "round_robin_scheduling_algorithm.h"
#include "../../Utils/include/configuration_manager.h"

t_scheduling_algorithm* round_robin_algorithm;
int maximum_quantum;
int quantum_consumed;

bool round_robin_can_handle(char* scheduling_algorithm_name){
    return string_equals_ignore_case(scheduling_algorithm_name, "RR");
}

void round_robin_update_ready_queue_when_adding_function(t_list* ready_trainer_thread_contexts, t_trainer_thread_context* trainer_thread_context){
    list_add(ready_trainer_thread_contexts, trainer_thread_context);
}

bool round_robin_should_execute_now_function(t_trainer_thread_context* trainer_thread_context){
    (void) trainer_thread_context;
    return basic_should_execute();
}

void initialize_quantum(){
    maximum_quantum = config_get_int_at("QUANTUM");
}

void round_robin_execution_cycle_consumed_function(){
    quantum_consumed++;

    if(quantum_consumed == maximum_quantum){
        preempt();
    }
}

void round_robin_reset_quantum_consumed_function(){
    quantum_consumed = 0;
}

void initialize_round_robin_scheduling_algorithm(){
    round_robin_algorithm = safe_malloc(sizeof(t_scheduling_algorithm));
    round_robin_algorithm -> can_handle_function = round_robin_can_handle;
    round_robin_algorithm -> update_ready_queue_when_adding_function = round_robin_update_ready_queue_when_adding_function;
    round_robin_algorithm -> should_execute_now_function = round_robin_should_execute_now_function;
    round_robin_algorithm -> execution_cycle_consumed_function = round_robin_execution_cycle_consumed_function;
    round_robin_algorithm -> reset_quantum_consumed_function = round_robin_reset_quantum_consumed_function;

    initialize_quantum();
    round_robin_reset_quantum_consumed_function();
}

t_scheduling_algorithm* round_robin_scheduling_algorithm(){
    return round_robin_algorithm;
}