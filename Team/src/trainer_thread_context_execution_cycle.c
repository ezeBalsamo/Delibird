#include <movement_action.h>
#include <stdlib.h>
#include <catch_action.h>
#include <goal_calculator.h>
#include <dispatcher.h>
#include <trainer_thread_context_state_chained_evaluation.h>
#include <team_logs_manager.h>
#include <waiting_actions.h>
#include "trainer_thread_context_execution_cycle.h"

void free_if_necessary_previous(t_thread_action* thread_action){
    if(thread_action != NULL){
        free_thread_action(thread_action);
    }
}

void prepare_for_movement_action(t_trainer_thread_context* trainer_thread_context, t_localizable_object* localizable_pokemon){

    free_if_necessary_previous(trainer_thread_context -> thread_action);

    trainer_thread_context -> thread_action =
            movement_thread_action_for(trainer_thread_context -> localizable_trainer, localizable_pokemon);

    trainer_thread_context_ready_to_be_sheduled(trainer_thread_context);
}

void movement_action_completed_by(t_trainer_thread_context* trainer_thread_context){

    t_movement_action* movement_action = internal_thread_action_in(trainer_thread_context);
    t_localizable_object* localizable_pokemon = movement_action -> destiny_object;

    free_thread_action(trainer_thread_context -> thread_action);

    trainer_thread_context -> thread_action = catch_thread_action_for(localizable_pokemon);

    log_thread_action_to_perform_by(trainer_thread_context);
    execute_trainer_thread_context_action(trainer_thread_context);
}

void catch_action_completed_by(t_trainer_thread_context* trainer_thread_context){

    t_catch_action* catch_action = internal_thread_action_in(trainer_thread_context);
    t_localizable_object* localizable_pokemon = catch_action -> localizable_pokemon;

    free_thread_action(trainer_thread_context -> thread_action);
    update_current_pokemons_after_caught(trainer_thread_context -> localizable_trainer, localizable_pokemon -> object);

    trainer_thread_context_state_chained_evaluation_value_when_caught_for(trainer_thread_context);
    remove_occurrence_of(localizable_pokemon);
}

void catch_action_blocked_in_wait_of_response(t_trainer_thread_context* trainer_thread_context, int message_id){

    t_catch_action* catch_action = internal_thread_action_in(trainer_thread_context);
    t_localizable_object* localizable_pokemon = catch_action -> localizable_pokemon;

    free_thread_action(trainer_thread_context -> thread_action);
    trainer_thread_context -> thread_action =
            waiting_catch_response_thread_action_for(localizable_pokemon, message_id);;

    trainer_thread_context_has_become_blocked(trainer_thread_context);
}

void prepare_for_waiting_for_more_pokemons_action(t_trainer_thread_context* trainer_thread_context){

    trainer_thread_context -> thread_action = waiting_for_more_pokemons_thread_action();
    trainer_thread_context_has_become_blocked(trainer_thread_context);
}

void trainer_thread_context_has_entered_deadlock_zone(t_trainer_thread_context* trainer_thread_context){
    //TODO implementar lógica de bloqueo por deadlock
}