#include <commons/collections/queue.h>
#include <team_logs_manager.h>
#include <scheduling_algorithm.h>
#include <dispatching_reasons.h>
#include <state_transitions.h>
#include "dispatcher.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/t_list_extension.h"

t_list* new_trainer_thread_contexts;
t_queue* ready_trainer_thread_contexts;
t_trainer_thread_context* trainer_thread_context_executing;
t_list* blocked_trainer_thread_contexts;
t_list* finished_trainer_thread_contexts;

bool must_preempt;

pthread_mutex_t schedulable_trainer_thread_contexts_mutex;
pthread_mutex_t ready_queue_mutex;
pthread_mutex_t execute_mutex;

void initialize_dispatcher(){
    new_trainer_thread_contexts = list_create();
    ready_trainer_thread_contexts = queue_create();
    blocked_trainer_thread_contexts = list_create();
    finished_trainer_thread_contexts = list_create();

    initialize_state_transitions();
    initialize_scheduling_algorithm();

    must_preempt = false;

    safe_mutex_initialize(&schedulable_trainer_thread_contexts_mutex);
    safe_mutex_initialize(&ready_queue_mutex);
}

void new_thread_created_for(t_trainer_thread_context* trainer_thread_context){
    list_add(new_trainer_thread_contexts, (void *) trainer_thread_context);
    log_trainer_added_to_new(trainer_thread_context -> localizable_trainer);
}

bool can_be_schedule(t_trainer_thread_context* trainer_thread_context){

    t_thread_action* thread_action = trainer_thread_context -> thread_action;
    return thread_action -> request -> operation == WAITING_FOR_MORE_POKEMONS;
}

bool cant_be_schedule(t_trainer_thread_context* trainer_thread_context){

    return !can_be_schedule(trainer_thread_context);
}

t_list* schedulable_blocked_trainer_thread_contexts(){
    return list_filter(blocked_trainer_thread_contexts, (bool (*)(void *)) can_be_schedule);
}


t_list* non_schedulable_blocked_trainer_thread_contexts(){
    return list_filter(blocked_trainer_thread_contexts, (bool (*)(void *)) cant_be_schedule);
}

t_list* schedulable_trainer_thread_contexts(){

    pthread_mutex_lock(&schedulable_trainer_thread_contexts_mutex);

    t_list* trainer_thread_contexts = list_create();
    list_add_all(trainer_thread_contexts, new_trainer_thread_contexts);

    t_list* available_to_schedule_blocked_trainer_thread_contexts = schedulable_blocked_trainer_thread_contexts();
    list_add_all(trainer_thread_contexts, available_to_schedule_blocked_trainer_thread_contexts);

    list_destroy(available_to_schedule_blocked_trainer_thread_contexts);
    return trainer_thread_contexts;
}

void remove_from(t_list* list_to_search, t_trainer_thread_context* trainer_thread_context_to_find){

    bool _are_equals(t_trainer_thread_context* trainer_thread_context_to_compare){
        return are_equal_trainers(trainer_thread_context_to_find -> localizable_trainer -> object,
                                  trainer_thread_context_to_compare -> localizable_trainer -> object);
    }

    t_trainer_thread_context* trainer_thread_context_found =
            list_remove_by_condition(list_to_search, (bool (*)(void *)) _are_equals);

    if (!trainer_thread_context_found) {
        t_trainer* trainer = trainer_thread_context_to_find -> localizable_trainer -> object;
        log_trainer_thread_context_not_found_error_for(trainer -> sequential_number);
        free_system();
    }
}

void consider_continue_executing(){
    if(!queue_is_empty(ready_trainer_thread_contexts)){
        execute_trainer_thread_context();
    }
}

void free_current_execution_doing(void (*state_function) ()){
    reset_quantum_consumed();
    state_function();
    trainer_thread_context_executing = NULL;
    pthread_mutex_unlock(&execute_mutex);
    consider_continue_executing();
}

void remove_from_new(t_trainer_thread_context* trainer_thread_context){
    remove_from(new_trainer_thread_contexts, trainer_thread_context);
}

void remove_from_blocked(t_trainer_thread_context* trainer_thread_context){
    remove_from(blocked_trainer_thread_contexts, trainer_thread_context);
}

void add_to_blocked(t_trainer_thread_context* trainer_thread_context){
    trainer_thread_context -> state = BLOCKED;
    list_add(blocked_trainer_thread_contexts, trainer_thread_context);
}

void add_to_finished(t_trainer_thread_context* trainer_thread_context){
    trainer_thread_context -> state = FINISHED;
    list_add(finished_trainer_thread_contexts, trainer_thread_context);
}

void schedule(t_trainer_thread_context* trainer_thread_context, char* reason){
    pthread_mutex_lock(&ready_queue_mutex);
    update_ready_queue_when_adding(ready_trainer_thread_contexts, trainer_thread_context);
    pthread_mutex_unlock(&ready_queue_mutex);

    log_trainer_schedule(trainer_thread_context -> localizable_trainer, reason);
    trainer_thread_context -> state = READY;

    t_state_transition* state_transition = state_transition_for(trainer_thread_context, EXECUTE);
    state_transition -> state_transition_function (trainer_thread_context);
}

void trainer_thread_context_ready_to_be_sheduled(t_trainer_thread_context* trainer_thread_context){

    t_state_transition* state_transition = state_transition_for(trainer_thread_context, READY);
    state_transition -> state_transition_function (trainer_thread_context);
}

bool is_anybody_executing(){
    return trainer_thread_context_executing != NULL;
}

void execute_trainer_thread_context(){

    pthread_mutex_lock(&execute_mutex);

    trainer_thread_context_executing = queue_pop(ready_trainer_thread_contexts);
    trainer_thread_context_executing -> state = EXECUTE;

    log_trainer_execution(trainer_thread_context_executing -> localizable_trainer,
                          thread_action_reason_for(trainer_thread_context_executing));

    sem_post(&trainer_thread_context_executing -> semaphore);
}

bool preemption_must_take_place(){
    return must_preempt;
}

void preempt(){
    must_preempt = true;
    t_state_transition* state_transition = state_transition_for(trainer_thread_context_executing, READY);
    state_transition -> state_transition_function (trainer_thread_context_executing);
    must_preempt = false;
}

void trainer_thread_context_has_finished(t_trainer_thread_context* trainer_thread_context){

    t_state_transition* state_transition = state_transition_for(trainer_thread_context, FINISHED);
    state_transition -> state_transition_function (trainer_thread_context);
}

void trainer_thread_context_has_become_blocked(t_trainer_thread_context* trainer_thread_context){

    t_state_transition* state_transition = state_transition_for(trainer_thread_context, BLOCKED);
    state_transition -> state_transition_function (trainer_thread_context);
}

void assert_all_trainer_thread_contexts_have_finished(){
    if(!list_is_empty(new_trainer_thread_contexts)){
        log_expected_to_be_empty_error_for("lista de nuevos");
        free_system();
    }
    if(!queue_is_empty(ready_trainer_thread_contexts)){
        log_expected_to_be_empty_error_for("cola de listos");
        free_system();
    }
    if(!list_is_empty(blocked_trainer_thread_contexts)){
        log_expected_to_be_empty_error_for("lista de bloqueados");
        free_system();
    }
    if(is_anybody_executing()){
        log_expected_no_trainer_thread_executing_error_for(trainer_thread_context_executing);
        free_system();
    }
    if(list_is_empty(finished_trainer_thread_contexts)){
        log_expected_to_be_not_empty_error_for("lista de finalizados");
        free_system();
    }
}

int finished_trainer_thread_contexts_amount(){
    return list_size(finished_trainer_thread_contexts);
}

void free_dispatcher(){
    list_destroy(new_trainer_thread_contexts);
    queue_destroy(ready_trainer_thread_contexts);
    list_destroy(blocked_trainer_thread_contexts);
    list_destroy(finished_trainer_thread_contexts);

    pthread_mutex_destroy(&execute_mutex);
    pthread_mutex_destroy(&schedulable_trainer_thread_contexts_mutex);
    pthread_mutex_destroy(&ready_queue_mutex);

    free_scheduling_algorithm();
    free_state_transitions();
}