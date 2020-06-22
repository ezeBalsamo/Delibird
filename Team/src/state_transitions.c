#include "state_transitions.h"
#include "../../Utils/include/garbage_collector.h"
#include <stdlib.h>
#include <team_logs_manager.h>
#include <dispatcher.h>
#include <dispatching_reasons.h>
#include <pthread.h>
#include <scheduling_algorithm.h>

t_list* state_transitions;
extern pthread_mutex_t schedulable_trainer_thread_contexts_mutex;

void new_to_ready_transition_function(t_trainer_thread_context* trainer_thread_context){
    remove_from_new(trainer_thread_context);
    pthread_mutex_unlock(&schedulable_trainer_thread_contexts_mutex);
    schedule(trainer_thread_context, thread_action_reason_for(trainer_thread_context));
}

void ready_to_execute_transition_function(t_trainer_thread_context* trainer_thread_context){
   /* if(should_execute(trainer_thread_context)){
        execute_trainer_thread_context();
    }
    */
}

void blocked_to_ready_transition_function(t_trainer_thread_context* trainer_thread_context){
    remove_from_blocked(trainer_thread_context);
    pthread_mutex_unlock(&schedulable_trainer_thread_contexts_mutex);
    schedule(trainer_thread_context, thread_action_reason_for(trainer_thread_context));
}

void execute_to_ready_transition_function(t_trainer_thread_context* trainer_thread_context){
    void _coming_from_execute_function(){
        schedule(trainer_thread_context, thread_action_reason_for(trainer_thread_context));
    }

    free_current_execution_doing(_coming_from_execute_function);
}

void execute_to_finished_transition_function(t_trainer_thread_context* trainer_thread_context){
    void _finished_function(){
        add_to_finished(trainer_thread_context);
        log_trainer_has_accomplished_own_goal(trainer_thread_context -> localizable_trainer);
    }

    free_current_execution_doing(_finished_function);
    consider_global_goal_accomplished();
}

void blocked_to_finished_transition_function(t_trainer_thread_context* trainer_thread_context){

    remove_from_blocked(trainer_thread_context);
    add_to_finished(trainer_thread_context);
    log_trainer_has_accomplished_own_goal(trainer_thread_context -> localizable_trainer);

    consider_global_goal_accomplished();
}

void execute_to_blocked_transition_function(t_trainer_thread_context* trainer_thread_context){
    void _blocked_function(){
        add_to_blocked(trainer_thread_context);
        log_trainer_blocked(trainer_thread_context);
    }

    free_current_execution_doing(_blocked_function);
    sem_wait(&trainer_thread_context -> semaphore);
}

void blocked_to_blocked_transition_function(t_trainer_thread_context* trainer_thread_context){

    log_trainer_blocked(trainer_thread_context);
    sem_wait(&trainer_thread_context -> semaphore);
}

void load_new_state_transition(uint32_t to_state, uint32_t from_state, void (*state_function) (t_trainer_thread_context*)){
    t_state_transition* state_transition = safe_malloc(sizeof(t_state_transition));
    state_transition -> from_state = from_state;
    state_transition -> to_state = to_state;
    state_transition -> state_transition_function = state_function;

    list_add(state_transitions, state_transition);
}

void initialize_state_transitions(){
    state_transitions = list_create();
    load_new_state_transition(NEW, READY, new_to_ready_transition_function);
    load_new_state_transition(READY, EXECUTE, ready_to_execute_transition_function);
    load_new_state_transition(EXECUTE, FINISHED, execute_to_finished_transition_function);
    load_new_state_transition(EXECUTE, READY, execute_to_ready_transition_function);
    load_new_state_transition(EXECUTE, BLOCKED, execute_to_blocked_transition_function);
    load_new_state_transition(BLOCKED, READY, blocked_to_ready_transition_function);
    load_new_state_transition(BLOCKED, BLOCKED, blocked_to_blocked_transition_function);
    load_new_state_transition(BLOCKED, FINISHED, blocked_to_finished_transition_function);
}

void free_state_transitions(){
    list_destroy_and_destroy_elements(state_transitions, free);
}

t_state_transition* state_transition_for(t_trainer_thread_context* trainer_thread_context, uint32_t to_state){

    bool _is_for(void* state_transition){
        t_state_transition* cast_state_transition = (t_state_transition*) state_transition;

        return
            cast_state_transition -> to_state == to_state &&
            cast_state_transition -> from_state == trainer_thread_context -> state;
    }

    t_state_transition* state_transition_found = list_find(state_transitions, _is_for);

    if(!state_transition_found){
        log_invalid_transition_error();
        free_system();
    }

    return state_transition_found;
}