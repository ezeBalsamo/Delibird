#include <pokemon_occurrence_trigger.h>
#include <dispatcher.h>
#include <team_logs_manager.h>
#include <trainer_threads.h>
#include <trainer_thread_context_execution_cycle.h>
#include <distance_calculator.h>

void chase(t_localizable_object* localizable_pokemon){

    t_list* trainer_thread_contexts = schedulable_trainer_thread_contexts();

    if(list_is_empty(trainer_thread_contexts)){
        log_no_schedulable_threads_available_for(localizable_pokemon -> object);
    }else{
        t_trainer_thread_context* trainer_thread_context =
                trainer_thread_context_closest_to(trainer_thread_contexts, localizable_pokemon);

        list_destroy(trainer_thread_contexts);
        prepare_for_movement_action(trainer_thread_context, localizable_pokemon);
    }
}