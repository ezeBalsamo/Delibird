#include <commons/string.h>
#include <dispatcher.h>
#include <event_notifier.h>
#include "round_robin_scheduling_algorithm.h"
#include "../../Utils/include/configuration_manager.h"

t_scheduling_algorithm* round_robin_algorithm;
int maximum_quantum;
int quantum_quantity_consumed;

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

void initialize_round_robin_scheduling_algorithm(){
    round_robin_algorithm = safe_malloc(sizeof(t_scheduling_algorithm));
    round_robin_algorithm -> can_handle_function = round_robin_can_handle;
    round_robin_algorithm -> update_ready_queue_when_adding_function = round_robin_update_ready_queue_when_adding_function;
    round_robin_algorithm -> should_execute_now_function = round_robin_should_execute_now_function;

    initialize_quantum();
    subscribe_to_events();
}

t_scheduling_algorithm* round_robin_scheduling_algorithm(){
    return round_robin_algorithm;
}