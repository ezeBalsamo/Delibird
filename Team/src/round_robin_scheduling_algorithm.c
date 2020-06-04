#include <commons/string.h>
#include "round_robin_scheduling_algorithm.h"
#include "../../Utils/include/common_structures.h"

t_scheduling_algorithm* round_robin_algorithm;

bool round_robin_can_handle(char* scheduling_algorithm_name){
    return string_equals_ignore_case(scheduling_algorithm_name, "RR");
}

void round_robin_update_ready_queue_when_adding_function(t_queue* ready_queue, t_trainer_thread_context* trainer_thread_context){
    queue_push(ready_queue, trainer_thread_context);
}

bool round_robin_should_execute_now_function(t_trainer_thread_context* trainer_thread_context){
    (void) trainer_thread_context;
    return basic_should_execute();
}

void initialize_round_robin_scheduling_algorithm(){
    round_robin_algorithm = safe_malloc(sizeof(t_scheduling_algorithm));
    round_robin_algorithm -> can_handle_function = round_robin_can_handle;
    round_robin_algorithm -> update_ready_queue_when_adding_function = round_robin_update_ready_queue_when_adding_function;
    round_robin_algorithm -> should_execute_now_function = round_robin_should_execute_now_function;
}

t_scheduling_algorithm* round_robin_scheduling_algorithm(){
    return round_robin_algorithm;
}