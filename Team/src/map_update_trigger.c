#include <map_update_trigger.h>
#include <dispatcher.h>
#include <team_logs_manager.h>
#include <trainer_threads.h>
#include "../../Utils/include/free_system.h"
#include <stdlib.h>
#include <movement_action.h>

void assert_not_empty(t_list* thread_contexts){

    if(list_is_empty(thread_contexts)){
        log_zero_schedulable_threads_error();
        free_system();
    }
}

uint32_t distance_between(t_localizable_object* localizable_object, t_localizable_object* another_localizable_object){
    uint32_t x_distance = abs(localizable_object -> pos_x - another_localizable_object -> pos_x);
    uint32_t y_distance = abs(localizable_object -> pos_y - another_localizable_object -> pos_y);

    return x_distance + y_distance;
}

void* closest_trainer_thread_context_to(t_trainer_thread_context* trainer_thread_context,
                         t_trainer_thread_context* another_trainer_thread_context, t_localizable_object* pokemon){

    uint32_t trainer_distance = distance_between(trainer_thread_context -> localizable_trainer, pokemon);
    uint32_t another_trainer_distance = distance_between(another_trainer_thread_context -> localizable_trainer, pokemon);

    return trainer_distance < another_trainer_distance ? trainer_thread_context : another_trainer_thread_context;
}

t_trainer_thread_context* schedulable_trainer_thread_context_closest_to(t_localizable_object* pokemon){

    t_list* trainer_thread_contexts = schedulable_trainer_thread_contexts();
    assert_not_empty(trainer_thread_contexts);

    t_trainer_thread_context* seed_trainer_thread_context = list_get(trainer_thread_contexts, 0);

    void* _closest_trainer_thread_context(void* trainer_thread_context, void* another_trainer_thread_context){
        t_trainer_thread_context* cast_trainer_thread_context = (t_trainer_thread_context*) trainer_thread_context;
        t_trainer_thread_context* cast_another_trainer_thread_context = (t_trainer_thread_context*) another_trainer_thread_context;

        return closest_trainer_thread_context_to(cast_trainer_thread_context, cast_another_trainer_thread_context, pokemon);
    }

    t_trainer_thread_context* trainer_thread_context = (t_trainer_thread_context*)
            list_fold(trainer_thread_contexts, seed_trainer_thread_context, _closest_trainer_thread_context);

    list_destroy(trainer_thread_contexts);
    return trainer_thread_context;
}

void map_updated_with_insertion_of(t_localizable_object* pokemon){

    t_trainer_thread_context* trainer_thread_context = schedulable_trainer_thread_context_closest_to(pokemon);

    t_movement_action* thread_action = movement_action_for(trainer_thread_context -> localizable_trainer, pokemon);

    trainer_thread_context -> thread_action = thread_action;
    trainer_thread_context -> execution_function = (void (*)(void *)) movement_action_execution_function;

    trainer_thread_context_ready_to_be_sheduled(trainer_thread_context);
}