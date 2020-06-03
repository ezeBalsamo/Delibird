#include <commons/collections/queue.h>
#include <team_logs_manager.h>
#include <scheduling_algorithm.h>
#include "dispatcher.h"
#include "../../Utils/include/free_system.h"

t_list* new_synchronizable_trainers;
t_queue* ready_synchronizable_trainers_found;
t_list* blocked_synchronizable_trainers;
t_list* finished_synchronizable_trainers;

void initialize_dispatcher(){
    new_synchronizable_trainers = list_create();
    ready_synchronizable_trainers_found = queue_create();
    blocked_synchronizable_trainers = list_create();
    finished_synchronizable_trainers = list_create();

    initialize_scheduling_algorithm();
}

void new_thread_created_for(t_synchronizable_trainer* synchronizable_trainer){
    list_add(new_synchronizable_trainers, (void *) synchronizable_trainer);
}

bool can_be_schedule(void* to_be_defined){
    // TODO implementar lógica de filtrado de hilos bloqueados
    return true;
}

t_list* schedulable_blocked_synchronizable_trainers(){
    return list_filter(blocked_synchronizable_trainers, can_be_schedule);
}

t_list* schedulable_synchronizable_trainers(){
    t_list* schedulable_synchronizable_trainers = list_create();
    list_add_all(schedulable_synchronizable_trainers, new_synchronizable_trainers);

    t_list* available_to_schedule_blocked_threads = schedulable_blocked_synchronizable_trainers();
    list_add_all(schedulable_synchronizable_trainers, available_to_schedule_blocked_threads);

    list_destroy(available_to_schedule_blocked_threads);
    return schedulable_synchronizable_trainers;
}

bool are_equals_synchronizable_trainers(t_synchronizable_trainer* synchronizable_trainer,
        t_synchronizable_trainer* another_synchronizable_trainer){

    return are_equal_trainers(synchronizable_trainer -> trainer, another_synchronizable_trainer -> trainer);
}

void trainer_ready_to_be_sheduled(t_synchronizable_trainer* synchronizable_trainer){

    bool _are_equals(void* synchronizable_trainer_to_compare){
        return are_equals_synchronizable_trainers(synchronizable_trainer_to_compare, synchronizable_trainer);
    }

    t_synchronizable_trainer* trainer_found =
            list_remove_by_condition(new_synchronizable_trainers, (bool (*)(void *)) _are_equals);

    if(!trainer_found) {
        trainer_found =
                list_remove_by_condition(blocked_synchronizable_trainers, (bool (*)(void *)) _are_equals);

        if (!trainer_found) {
            log_synchronizable_trainer_not_found_error_for(synchronizable_trainer -> trainer -> sequential_number);
            free_system();
        }
    }

    queue_push(ready_synchronizable_trainers_found, synchronizable_trainer);

}