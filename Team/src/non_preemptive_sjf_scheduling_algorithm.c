#include "non_preemptive_sjf_scheduling_algorithm.h"
#include <basic_sjf_scheduling_algorithm_behaviour.h>

t_scheduling_algorithm* non_preemptive_sjf_algorithm;

void non_preemptive_sjf_resolve_dependencies_function(){
    initialize_basic_sjf_scheduling_algorithm_behaviour();
}

void initialize_non_preemptive_sjf_scheduling_algorithm(){
    non_preemptive_sjf_algorithm = safe_malloc(sizeof(t_scheduling_algorithm));
    non_preemptive_sjf_algorithm -> algorithm_name = "SJF-SD";
    non_preemptive_sjf_algorithm -> update_ready_queue_when_adding_function = basic_sjf_update_ready_queue_when_adding;
    non_preemptive_sjf_algorithm -> resolve_dependencies_function = non_preemptive_sjf_resolve_dependencies_function;
}

t_scheduling_algorithm* non_preemptive_sjf_scheduling_algorithm(){
    return non_preemptive_sjf_algorithm;
}