#include <non_preemptive_sjf_scheduling_algorithm.h>
#include <basic_sjf_scheduling_algorithm_behaviour.h>
#include <dispatcher.h>
#include <event_notifier.h>
#include "preemptive_sjf_scheduling_algorithm.h"
#include "../../Utils/include/t_list_extension.h"

t_scheduling_algorithm* preemptive_sjf_algorithm;
t_trainer_thread_context* shortest_trainer_thread_context = NULL;
bool should_consider_preemption = false;

void preemptive_sjf_update_ready_queue_when_adding(t_list* ready_trainer_thread_contexts, t_trainer_thread_context* trainer_thread_context){

    basic_sjf_update_ready_queue_when_adding(ready_trainer_thread_contexts, trainer_thread_context);

    if(are_equal_trainer_thread_contexts(list_first(ready_trainer_thread_contexts), trainer_thread_context)){

        shortest_trainer_thread_context = trainer_thread_context;

        if (is_anybody_executing()){
            should_consider_preemption = true;
        }
    }
}

void consider_preemption_with(t_trainer* trainer){

    if(should_consider_preemption){

        double current_estimation_of_shortest_context = current_estimation_of_trainer_thread_context(shortest_trainer_thread_context);
        double remaining_estimation_of_trainer = remaining_estimation_of(trainer);

        if(current_estimation_of_shortest_context < remaining_estimation_of_trainer){
            t_trainer_remaining_time_estimator* trainer_remaining_time_estimator = trainer_remaining_time_estimator_of(trainer);
            trainer_remaining_time_estimator -> current_estimation = remaining_estimation_of_trainer;
            preempt();
        }
    }

    should_consider_preemption = false;
}

static void subscribe_to_events(){
    subscribe_to_event_doing(EXECUTION_CYCLE_CONSUMED_BY_TRAINER, (void (*)(void *)) consider_preemption_with);
}

void preemptive_sjf_resolve_dependencies_function(){
    initialize_basic_sjf_scheduling_algorithm_behaviour();
    subscribe_to_events();
}

void initialize_preemptive_sjf_scheduling_algorithm(){
    preemptive_sjf_algorithm = safe_malloc(sizeof(t_scheduling_algorithm));
    preemptive_sjf_algorithm -> algorithm_name = "SJF-CD";
    preemptive_sjf_algorithm -> update_ready_queue_when_adding_function = preemptive_sjf_update_ready_queue_when_adding;
    preemptive_sjf_algorithm -> resolve_dependencies_function = preemptive_sjf_resolve_dependencies_function;
}

t_scheduling_algorithm* preemptive_sjf_scheduling_algorithm(){
    return preemptive_sjf_algorithm;
}