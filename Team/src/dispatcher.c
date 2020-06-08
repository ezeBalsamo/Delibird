#include <commons/collections/queue.h>
#include <team_logs_manager.h>
#include <scheduling_algorithm.h>
#include <dispatching_reasons.h>
#include "dispatcher.h"
#include "../../Utils/include/free_system.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/pthread_wrapper.h"

t_list* new_trainer_thread_contexts;
t_queue* ready_trainer_thread_contexts;
t_trainer_thread_context* trainer_thread_context_executing;
t_list* blocked_trainer_thread_contexts;
t_list* finished_trainer_thread_contexts;

pthread_mutex_t schedulable_trainer_thread_contexts_mutex;
pthread_mutex_t ready_queue_mutex;
pthread_mutex_t execute_mutex;

void initialize_dispatcher(){
    new_trainer_thread_contexts = list_create();
    ready_trainer_thread_contexts = queue_create();
    blocked_trainer_thread_contexts = list_create();
    finished_trainer_thread_contexts = list_create();

    initialize_scheduling_algorithm();

    safe_mutex_initialize(&schedulable_trainer_thread_contexts_mutex);
    safe_mutex_initialize(&ready_queue_mutex);
}

void new_thread_created_for(t_trainer_thread_context* trainer_thread_context){
    trainer_thread_context -> state = NEW;
    list_add(new_trainer_thread_contexts, (void *) trainer_thread_context);
    log_trainer_added_to_new(trainer_thread_context -> localizable_trainer);
}

bool can_be_schedule(void* to_be_defined){
    // TODO implementar lógica de filtrado de hilos bloqueados
    return true;
}

t_list* schedulable_blocked_trainer_thread_contexts(){
    return list_filter(blocked_trainer_thread_contexts, can_be_schedule);
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
        log_synchronizable_trainer_not_found_error_for(trainer -> sequential_number);
        free_system();
    }
}

void remove_from_new_or_blocked(t_trainer_thread_context* trainer_thread_context){

    if(trainer_thread_context -> state == NEW){
        remove_from(new_trainer_thread_contexts, trainer_thread_context);
    }

    if (trainer_thread_context -> state == BLOCKED){
        remove_from(blocked_trainer_thread_contexts, trainer_thread_context);
    }
}

void schedule(t_trainer_thread_context* trainer_thread_context, char* reason){
    pthread_mutex_lock(&ready_queue_mutex);
    update_ready_queue_when_adding(ready_trainer_thread_contexts, trainer_thread_context);
    pthread_mutex_unlock(&ready_queue_mutex);

    log_trainer_schedule(trainer_thread_context -> localizable_trainer, reason);
    trainer_thread_context -> state = READY;

    trainer_thread_context_ready(trainer_thread_context);
}

void trainer_thread_context_ready_to_be_sheduled(t_trainer_thread_context* trainer_thread_context){

    remove_from_new_or_blocked(trainer_thread_context);
    pthread_mutex_unlock(&schedulable_trainer_thread_contexts_mutex);

    schedule(trainer_thread_context, move_to_pokemon_reason_for(trainer_thread_context));
}

bool is_anybody_executing(){
    return trainer_thread_context_executing != NULL;
}

void execute_trainer_thread_context(){

    pthread_mutex_lock(&execute_mutex);

    trainer_thread_context_executing = queue_pop(ready_trainer_thread_contexts);
    trainer_thread_context_executing -> state = EXECUTE;

    log_trainer_execution(trainer_thread_context_executing -> localizable_trainer,
                          thread_action_as_string(trainer_thread_context_executing));

    sem_post(&trainer_thread_context_executing -> semaphore);

    /*Todo lógica para esperar a que ejecute + lógica para ver que pasó
     * En el caso de un appeared/localized,
     *      -> si la conexión se realiza: va a ser bloqueado a la espera del caught
     *      -> si la conexión no se realiza: comportamiento por default, lo cual puede ser blocked o finished
     * En el caso de la resolución del deadlock, puede pasar a finished
     * */
}

void preempt_due_to(char* preemption_reason){
    t_trainer_thread_context* trainer_thread_context_to_schedule = trainer_thread_context_executing;
    trainer_thread_context_executing = NULL;
    pthread_mutex_unlock(&execute_mutex);
    schedule(trainer_thread_context_to_schedule, preemption_reason);
}

void consider_continue_executing(){
    if(!queue_is_empty(ready_trainer_thread_contexts)){
        execute_trainer_thread_context();
    }
}

void trainer_thread_context_has_finished(t_trainer_thread_context* trainer_thread_context){

    trainer_thread_context_executing = NULL;
    trainer_thread_context -> state = FINISHED;

    list_add(finished_trainer_thread_contexts, trainer_thread_context);
    log_trainer_has_accomplished_own_goal(trainer_thread_context -> localizable_trainer);

    pthread_mutex_unlock(&execute_mutex);
    consider_global_goal_accomplished();
    consider_continue_executing();
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
    if(trainer_thread_context_executing != NULL){
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

    free_scheduling_algorithm();
}