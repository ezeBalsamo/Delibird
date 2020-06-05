#include <map_update_trigger.h>
#include <dispatcher.h>
#include <team_logs_manager.h>
#include <trainer_threads.h>
#include "../../Utils/include/free_system.h"
#include <trainer_thread_context_execution_cycle.h>
#include <distance_calculator.h>

void assert_not_empty(t_list* thread_contexts){

    if(list_is_empty(thread_contexts)){
        log_zero_schedulable_threads_error();
        free_system();
    }
}

t_trainer_thread_context* schedulable_trainer_thread_context_closest_to(t_localizable_object* localizable_pokemon){

    t_list* trainer_thread_contexts = schedulable_trainer_thread_contexts();
    assert_not_empty(trainer_thread_contexts);

    t_trainer_thread_context* trainer_thread_context =
            trainer_thread_context_closest_to(trainer_thread_contexts, localizable_pokemon);

    list_destroy(trainer_thread_contexts);
    return trainer_thread_context;
}

void map_updated_with_insertion_of(t_localizable_object* localizable_pokemon){

    t_trainer_thread_context* trainer_thread_context = schedulable_trainer_thread_context_closest_to(localizable_pokemon);
    prepare_for_movement_action(trainer_thread_context, localizable_pokemon);
}