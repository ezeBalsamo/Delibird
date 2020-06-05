#include <commons/collections/queue.h>
#include <team_logs_manager.h>
#include <scheduling_algorithm.h>
#include "dispatcher.h"
#include "../../Utils/include/free_system.h"

t_list* new_trainer_thread_contexts;
t_queue* ready_trainer_thread_contexts;
t_list* blocked_trainer_thread_contexts;
t_list* finished_trainer_thread_contexts;

bool is_someone_executing = false;

void initialize_dispatcher(){
    new_trainer_thread_contexts = list_create();
    ready_trainer_thread_contexts = queue_create();
    blocked_trainer_thread_contexts = list_create();
    finished_trainer_thread_contexts = list_create();

    initialize_scheduling_algorithm();
}

void new_thread_created_for(t_trainer_thread_context* trainer_thread_context){
    trainer_thread_context -> state = NEW;
    list_add(new_trainer_thread_contexts, (void *) trainer_thread_context);
}

bool can_be_schedule(void* to_be_defined){
    // TODO implementar lógica de filtrado de hilos bloqueados
    return true;
}

t_list* schedulable_blocked_trainer_thread_contexts(){
    return list_filter(blocked_trainer_thread_contexts, can_be_schedule);
}

t_list* schedulable_trainer_thread_contexts(){
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

void trainer_thread_context_ready_to_be_sheduled(t_trainer_thread_context* trainer_thread_context){

    remove_from_new_or_blocked(trainer_thread_context);

    update_ready_queue_when_adding(ready_trainer_thread_contexts, trainer_thread_context);
    trainer_thread_context -> state = READY;
    trainer_thread_context_ready(trainer_thread_context);
}

bool is_anybody_executing(){
    return is_someone_executing;
}

void remove_from_ready(t_trainer_thread_context* trainer_thread_context){
    remove_from(ready_trainer_thread_contexts -> elements, trainer_thread_context);
}

void execute(t_trainer_thread_context* trainer_thread_context){

    //TODO semaforizar
    remove_from_ready(trainer_thread_context);
    is_someone_executing = true;
    trainer_thread_context -> state = EXECUTE;
    sem_post(&trainer_thread_context -> semaphore);

    /*Todo lógica para esperar a que ejecute + lógica para ver que pasó
     * En el caso de un appeared/localized,
     *      -> si la conexión se realiza: va a ser bloqueado a la espera del caught
     *      -> si la conexión no se realiza: comportamiento por default, lo cual puede ser blocked o finished
     * En el caso de la resolución del deadlock, puede pasar a finished
     * */
}

void trainer_thread_context_has_finished(t_trainer_thread_context* trainer_thread_context){
    //TODO
    trainer_thread_context -> state = FINISHED;
    list_add(finished_trainer_thread_contexts, trainer_thread_context);
}