#include <movement_action.h>
#include <stdlib.h>
#include <catch_action.h>
#include <goal_calculator.h>
#include <dispatcher.h>
#include <trainer_thread_context_state_chained_evaluation.h>
#include "trainer_thread_context_execution_cycle.h"

void prepare_for_movement_action(t_trainer_thread_context* trainer_thread_context, t_localizable_object* localizable_pokemon){

    t_movement_action* thread_action = movement_action_for(trainer_thread_context -> localizable_trainer, localizable_pokemon);

    trainer_thread_context -> thread_action = thread_action;
    trainer_thread_context -> execution_function = (void (*)(void *)) movement_action_execution_function;

    trainer_thread_context_ready_to_be_sheduled(trainer_thread_context);
}

void movement_action_completed_by(t_trainer_thread_context* trainer_thread_context){

    t_movement_action* thread_action = trainer_thread_context -> thread_action;
    t_localizable_object* localizable_pokemon = thread_action -> destiny_object;

    free(thread_action);

    trainer_thread_context -> thread_action = catch_action_for(localizable_pokemon);
    trainer_thread_context -> execution_function = (void (*)(void *)) catch_action_execution_function;

    execute_trainer_thread_context_action(trainer_thread_context);
}

void catch_action_completed_by(t_trainer_thread_context* trainer_thread_context){

    t_catch_action* catch_action = trainer_thread_context -> thread_action;
    t_localizable_object* localizable_pokemon = catch_action -> localizable_pokemon;

    free(catch_action);
    update_required_pokemons_after_catching(trainer_thread_context -> localizable_trainer, localizable_pokemon -> object);
    remove_pokemon_from_map(localizable_pokemon);

    trainer_thread_context_state_chained_evaluation_value_when_caught_for(trainer_thread_context);
}