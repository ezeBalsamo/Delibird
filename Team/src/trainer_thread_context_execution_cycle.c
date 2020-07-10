#include <movement_action.h>
#include <stdlib.h>
#include <catch_action.h>
#include <goal_calculator.h>
#include <dispatcher.h>
#include <trainer_thread_context_state_chained_evaluation.h>
#include <team_logs_manager.h>
#include <waiting_actions.h>
#include <exchange_action.h>
#include "trainer_thread_context_execution_cycle.h"
#include "../../Utils/include/t_list_extension.h"

void prepare_for_movement_action(t_trainer_thread_context* trainer_thread_context, t_localizable_object* destiny_object){

    free_thread_action(trainer_thread_context -> thread_action);

    trainer_thread_context -> thread_action =
            movement_thread_action_for(trainer_thread_context -> localizable_trainer, destiny_object);

    trainer_thread_context_ready_to_be_sheduled(trainer_thread_context);
}

void movement_action_completed_by(t_trainer_thread_context* trainer_thread_context){

    t_movement_action* movement_action = internal_thread_action_in(trainer_thread_context);
    t_localizable_object* destiny_object = movement_action -> destiny_object;

    free_thread_action(trainer_thread_context -> thread_action);

    if(is_deadlock_resolution_in_process()){
        trainer_thread_context -> thread_action = exchange_thread_action();
        log_thread_action_to_perform_by(trainer_thread_context);
        sem_post(&trainer_thread_context -> semaphore);
    }else{
        trainer_thread_context -> thread_action = catch_thread_action_for(destiny_object);
        log_thread_action_to_perform_by(trainer_thread_context);
        execute_trainer_thread_context_action(trainer_thread_context);
    }
}

void catch_action_completed_successfully_by(t_trainer_thread_context* trainer_thread_context){

    t_catch_action* catch_action = internal_thread_action_in(trainer_thread_context);
    t_localizable_object* localizable_pokemon = catch_action -> localizable_pokemon;

    update_current_pokemons_after_caught(trainer_thread_context -> localizable_trainer, localizable_pokemon -> object);

    trainer_thread_context_state_chained_evaluation_value_when_caught_success_for(trainer_thread_context);
    remove_occurrence_of(localizable_pokemon);
}

void catch_action_failed_by(t_trainer_thread_context* trainer_thread_context){
    trainer_thread_context_state_chained_evaluation_value_when_caught_failed_for(trainer_thread_context);
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

    free_thread_action(trainer_thread_context -> thread_action);
    trainer_thread_context -> thread_action = waiting_for_more_pokemons_thread_action();
    trainer_thread_context_has_become_blocked(trainer_thread_context);
}

void prepare_for_waiting_for_deadlock_resolution(t_trainer_thread_context* trainer_thread_context){
    
    free_thread_action(trainer_thread_context -> thread_action);
    t_trainer* trainer = trainer_thread_context -> localizable_trainer -> object;
    trainer_thread_context -> thread_action = waiting_for_exchange_thread_action_for(trainer);
    trainer_thread_context_has_become_blocked(trainer_thread_context);
}


void consider_graceful_finished_of(t_trainer_thread_context* trainer_thread_context){

    if(trainer_thread_context -> state == FINISHED){
        free_thread_action(trainer_thread_context -> thread_action);
        t_thread_action* null_thread_action = new_null_thread_action();
        trainer_thread_context -> thread_action = null_thread_action;
        sem_post(&trainer_thread_context -> semaphore);
    }
}

void exchange_action_completed_using(t_list* identified_exchanges){

    /* Sin importar el intercambio, me interesa recalcular el estado de ambos contextos,
     * ya sea para transicionar de ejecución a bloqueado, en el caso de un intercambio beneficioso
     * para la segunda parte, así como recalcular los pokemones requeridos y no capturados y
     * los pokemones en exceso, en el caso de un intercambio beneficioso para la primera parte.
     */

    t_identified_exchange* identified_exchange = list_first(identified_exchanges);
    t_trainer_thread_context* first_party_trainer_thread_context =
            identified_exchange -> exchange -> first_party_trainer_thread_context;
    t_trainer_thread_context* second_party_trainer_thread_context =
            identified_exchange -> exchange -> second_party_trainer_thread_context;

    trainer_thread_context_state_chained_evaluation_value_when_caught_success_for(first_party_trainer_thread_context);
    trainer_thread_context_state_chained_evaluation_value_when_caught_success_for(second_party_trainer_thread_context);
    consider_graceful_finished_of(second_party_trainer_thread_context);
}