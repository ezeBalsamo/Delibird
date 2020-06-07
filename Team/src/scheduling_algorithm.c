#include <scheduling_algorithm.h>
#include <scheduling_algorithm_finder.h>
#include <dispatcher.h>

t_scheduling_algorithm* scheduling_algorithm;

void initialize_scheduling_algorithm(){
    scheduling_algorithm = chosen_scheduling_algorithm();
}

void update_ready_queue_when_adding(t_queue* ready_queue, t_trainer_thread_context* trainer_thread_context){
    scheduling_algorithm -> update_ready_queue_when_adding_function (ready_queue, trainer_thread_context);
}

bool basic_should_execute(){
    return !is_anybody_executing();
}

bool should_execute(t_trainer_thread_context* trainer_thread_context){
    return scheduling_algorithm -> should_execute_now_function (trainer_thread_context);
}

void trainer_thread_context_ready(t_trainer_thread_context* trainer_thread_context){

    if(should_execute(trainer_thread_context)){
        execute();
    }
}