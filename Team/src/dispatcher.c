#include <commons/collections/queue.h>
#include <team_logs_manager.h>
#include <scheduling_algorithm.h>
#include <dispatching_reasons.h>
#include <state_transitions.h>
#include <dispatcher_queues.h>
#include <deadlock_detection_and_recovery_algorithm.h>
#include <event_notifier.h>
#include "dispatcher.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/t_list_extension.h"

bool must_preempt;
bool deadlock_resolution_in_process;
pthread_mutex_t schedulable_trainer_thread_contexts_mutex;
pthread_mutex_t execute_mutex;

void initialize_dispatcher(){

    initialize_dispatcher_queues();
    initialize_state_transitions();
    initialize_scheduling_algorithm();
    initialize_deadlock_detection_and_recovery_algorithm();

    must_preempt = false;
    deadlock_resolution_in_process = false;
    safe_mutex_initialize(&schedulable_trainer_thread_contexts_mutex);
    safe_mutex_initialize(&execute_mutex);
}

void new_thread_created_for(t_trainer_thread_context* trainer_thread_context){
    add_to_dispatcher_queue(trainer_thread_context, NEW);
    log_trainer_added_to_new(trainer_thread_context -> localizable_trainer);
}

bool can_be_schedule(t_trainer_thread_context* trainer_thread_context){

    return internal_thread_action_type_in(trainer_thread_context) == WAITING_FOR_MORE_POKEMONS;
}

bool cant_be_schedule(t_trainer_thread_context* trainer_thread_context){

    return !can_be_schedule(trainer_thread_context);
}

t_list* schedulable_blocked_trainer_thread_contexts(){
    t_list* blocked_trainer_thread_contexts = trainer_thread_contexts_in(BLOCKED);
    return list_filter(blocked_trainer_thread_contexts, (bool (*)(void *)) can_be_schedule);
}


t_list* non_schedulable_blocked_trainer_thread_contexts(){
    t_list* blocked_trainer_thread_contexts = trainer_thread_contexts_in(BLOCKED);
    return list_filter(blocked_trainer_thread_contexts, (bool (*)(void *)) cant_be_schedule);
}

t_list* schedulable_trainer_thread_contexts(){

    pthread_mutex_lock(&schedulable_trainer_thread_contexts_mutex);

    t_list* trainer_thread_contexts = list_create();

    t_list* new_trainer_thread_contexts = trainer_thread_contexts_in(NEW);
    list_add_all(trainer_thread_contexts, new_trainer_thread_contexts);

    t_list* available_to_schedule_blocked_trainer_thread_contexts = schedulable_blocked_trainer_thread_contexts();
    list_add_all(trainer_thread_contexts, available_to_schedule_blocked_trainer_thread_contexts);

    list_destroy(available_to_schedule_blocked_trainer_thread_contexts);
    return trainer_thread_contexts;
}

void consider_continue_executing(){

    t_list* trainer_thread_contexts = trainer_thread_contexts_in(READY);
    if(!list_is_empty(trainer_thread_contexts)){
        execute_trainer_thread_context();
    }
}

void stop_current_execution_doing(void (*state_function) ()){
    notify(CURRENT_EXECUTION_STOPPED);
    state_function();
    remove_from_execute();
    pthread_mutex_unlock(&execute_mutex);
    consider_continue_executing();
}

void move_to_ready_according_scheduling_algorithm(t_trainer_thread_context* trainer_thread_context){

    t_dispatcher_queue* dispatcher_queue = dispatcher_queue_of(READY);

    pthread_mutex_lock(&dispatcher_queue -> mutex);
    update_ready_queue_when_adding(dispatcher_queue -> trainer_thread_contexts, trainer_thread_context);
    pthread_mutex_unlock(&dispatcher_queue -> mutex);

    trainer_thread_context -> state = READY;
}

void schedule(t_trainer_thread_context* trainer_thread_context, char* reason){

    move_to_ready_according_scheduling_algorithm(trainer_thread_context);
    log_trainer_schedule(trainer_thread_context -> localizable_trainer, reason);

    t_state_transition* state_transition = state_transition_for(trainer_thread_context, EXECUTE);
    state_transition -> state_transition_function (trainer_thread_context);
}

void trainer_thread_context_ready_to_be_sheduled(t_trainer_thread_context* trainer_thread_context){

    t_state_transition* state_transition = state_transition_for(trainer_thread_context, READY);
    state_transition -> state_transition_function (trainer_thread_context);
}

void assert_only_one_executing(){
    t_list* trainer_thread_contexts = trainer_thread_contexts_in(EXECUTE);
    if(list_size(trainer_thread_contexts) != 1){
        log_more_than_one_trainer_thread_context_executing_error_for(trainer_thread_contexts);
        free_system();
    }
}

t_trainer_thread_context* trainer_thread_context_executing(){
    assert_only_one_executing();
    return list_first(trainer_thread_contexts_in(EXECUTE));
}

bool is_anybody_executing(){
    t_list* trainer_thread_contexts = trainer_thread_contexts_in(EXECUTE);
    return !list_is_empty(trainer_thread_contexts);
}

void execute_trainer_thread_context(){

    pthread_mutex_lock(&execute_mutex);

    move_to_execute();
    t_trainer_thread_context* trainer_thread_context = trainer_thread_context_executing();

    log_trainer_execution(trainer_thread_context -> localizable_trainer,
            thread_action_reason_for(trainer_thread_context));

    sem_post(&trainer_thread_context -> semaphore);
}

bool preemption_must_take_place(){
    return must_preempt;
}

void preempt(){
    must_preempt = true;
    t_trainer_thread_context* trainer_thread_context = trainer_thread_context_executing();
    t_state_transition* state_transition = state_transition_for(trainer_thread_context, READY);
    state_transition -> state_transition_function (trainer_thread_context);
}

void preemption_completed(){
    must_preempt = false;
}

void trainer_thread_context_has_finished(t_trainer_thread_context* trainer_thread_context){

    t_state_transition* state_transition = state_transition_for(trainer_thread_context, FINISHED);
    state_transition -> state_transition_function (trainer_thread_context);
}

void consider_deadlock_occurred_according_to(t_trainer_thread_context* trainer_thread_context){

    uint32_t thread_action_type = internal_thread_action_type_in(trainer_thread_context);
    if(thread_action_type == WAITING_FOR_EXCHANGE && !is_deadlock_resolution_in_process()){
        detect_and_recover_from_deadlock();
    }
}

void trainer_thread_context_has_become_blocked(t_trainer_thread_context* trainer_thread_context){

    t_state_transition* state_transition = state_transition_for(trainer_thread_context, BLOCKED);
    state_transition -> state_transition_function (trainer_thread_context);

    consider_deadlock_occurred_according_to(trainer_thread_context);
}

bool is_deadlock_resolution_in_process(){
    return deadlock_resolution_in_process;
}

void deadlock_solver_has_begun(){
    deadlock_resolution_in_process = true;
}

void deadlock_solver_has_ended(){
    deadlock_resolution_in_process = false;
    consider_ending();
}

void consider_ending(){
    if(trainers_amount() == finished_trainer_thread_contexts_amount() && !is_deadlock_resolution_in_process()){
        all_trainer_threads_context_have_finished();
    }
}

int finished_trainer_thread_contexts_amount(){

    t_list* trainer_thread_contexts = trainer_thread_contexts_in(FINISHED);
    return list_size(trainer_thread_contexts);
}

void free_dispatcher(){

    pthread_mutex_destroy(&execute_mutex);
    pthread_mutex_destroy(&schedulable_trainer_thread_contexts_mutex);
    free_dispatcher_queues();
    free_scheduling_algorithm();
    free_state_transitions();
    free_deadlock_detection_and_recovery_algorithm();
}