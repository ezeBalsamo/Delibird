#include <scheduling_algorithm.h>
#include <scheduling_algorithm_finder.h>
#include <dispatcher.h>
#include <stdlib.h>

t_scheduling_algorithm* scheduling_algorithm;

void initialize_scheduling_algorithm(){
    scheduling_algorithm = chosen_scheduling_algorithm();
    scheduling_algorithm -> resolve_dependencies_function();
}

void update_ready_queue_when_adding(t_list* ready_trainer_thread_contexts, t_trainer_thread_context* trainer_thread_context){
    scheduling_algorithm -> update_ready_queue_when_adding_function (ready_trainer_thread_contexts, trainer_thread_context);
}

char* scheduling_algorithm_name(){
    return scheduling_algorithm -> algorithm_name;
}

void free_scheduling_algorithm(){
    free(scheduling_algorithm);
}