#include <commons/string.h>
#include "preemptive_sjf_scheduling_algorithm.h"

t_scheduling_algorithm* preemptive_sjf_algorithm;

bool preemptive_sjf_can_handle(char* scheduling_algorithm_name){
    return string_equals_ignore_case(scheduling_algorithm_name, "SJF-CD");
}

void preemptive_sjf_update_ready_queue_when_adding_function(t_list* ready_trainer_thread_contexts, t_trainer_thread_context* trainer_thread_context){
    //TODO lógica en la que me va ayudar el groso de nico
}

bool preemptive_sjf_should_execute_now_function(t_trainer_thread_context* trainer_thread_context){
    //TODO: basic_should_execute || ver que el estimador del peek de la cola de listos sea menor que el que está en ejecución
}


void initialize_preemptive_sjf_scheduling_algorithm(){
    preemptive_sjf_algorithm = safe_malloc(sizeof(t_scheduling_algorithm));
    preemptive_sjf_algorithm -> can_handle_function = preemptive_sjf_can_handle;
    preemptive_sjf_algorithm -> update_ready_queue_when_adding_function = preemptive_sjf_update_ready_queue_when_adding_function;
    preemptive_sjf_algorithm -> should_execute_now_function = preemptive_sjf_should_execute_now_function;
}

t_scheduling_algorithm* preemptive_sjf_scheduling_algorithm(){
    return preemptive_sjf_algorithm;
}