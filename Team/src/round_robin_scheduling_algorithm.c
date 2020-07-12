#include <dispatcher.h>
#include <event_notifier.h>
#include "round_robin_scheduling_algorithm.h"
#include "../../Utils/include/configuration_manager.h"

t_scheduling_algorithm* round_robin_algorithm;
int maximum_quantum;
int quantum_quantity_consumed;

void round_robin_update_ready_queue_when_adding_function(t_list* ready_trainer_thread_contexts, t_trainer_thread_context* trainer_thread_context){
    list_add(ready_trainer_thread_contexts, trainer_thread_context);
}

void reset_quantum_consumed(){
    quantum_quantity_consumed = 0;
}

void initialize_quantum(){
    maximum_quantum = config_get_int_at("QUANTUM");
    reset_quantum_consumed();
}

void quantum_consumed(){
    quantum_quantity_consumed++;

    if(quantum_quantity_consumed == maximum_quantum){
        preempt();
    }
}

static void subscribe_to_events(){
    // No se incluye subscribe_to_event_doing(EXECUTION_CYCLE_CONSUMED, quantum_consumed);
    // Debido a que el único que lo envía es GET y esto causa problemas con el quantum.

    subscribe_to_event_doing(EXECUTION_CYCLE_CONSUMED_BY_TRAINER, quantum_consumed);
    subscribe_to_event_doing(CONTEXT_SWITCH_REALIZED, reset_quantum_consumed);
}

void round_robin_resolve_dependencies_function(){
    subscribe_to_events();
}

void initialize_round_robin_scheduling_algorithm(){
    round_robin_algorithm = safe_malloc(sizeof(t_scheduling_algorithm));
    round_robin_algorithm -> algorithm_name = "RR";
    round_robin_algorithm -> update_ready_queue_when_adding_function = round_robin_update_ready_queue_when_adding_function;
    round_robin_algorithm -> resolve_dependencies_function = round_robin_resolve_dependencies_function;

    initialize_quantum();
}

t_scheduling_algorithm* round_robin_scheduling_algorithm(){
    return round_robin_algorithm;
}