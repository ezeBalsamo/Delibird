#include <pokemon_occurrence_trigger.h>
#include <dispatcher.h>
#include <team_logs_manager.h>
#include <trainer_threads.h>
#include <trainer_thread_context_execution_cycle.h>
#include <distance_calculator.h>
#include "../../Utils/include/pthread_wrapper.h"

extern pthread_mutex_t schedulable_trainer_thread_contexts_mutex;

void chase(t_targetable_object* targetable_pokemon){

    t_localizable_object* localizable_pokemon = targetable_pokemon -> localizable_pokemon;

    t_list* trainer_thread_contexts = schedulable_trainer_thread_contexts();

    if(list_is_empty(trainer_thread_contexts)){
        log_no_schedulable_threads_available_for(localizable_pokemon -> object);
        safe_mutex_unlock(&schedulable_trainer_thread_contexts_mutex);
    }else{
        t_trainer_thread_context* trainer_thread_context =
                trainer_thread_context_closest_to_localizable_object(trainer_thread_contexts, localizable_pokemon);
        targetable_pokemon -> is_being_targeted = true;
        prepare_for_movement_action(trainer_thread_context, localizable_pokemon);
        safe_mutex_unlock(&schedulable_trainer_thread_contexts_mutex);
    }

    list_destroy(trainer_thread_contexts);
}