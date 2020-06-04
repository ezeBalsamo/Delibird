#include <movement_action.h>
#include <stdlib.h>
#include <catch_action.h>
#include "../include/trainer_thread_context_execution_cycle.h"

void movement_action_completed_by(t_trainer_thread_context* trainer_thread_context){

    t_movement_action* thread_action = trainer_thread_context -> thread_action;
    t_localizable_object* localizable_pokemon = thread_action -> destiny_object;

    free(thread_action);

    trainer_thread_context -> thread_action = catch_action_for(localizable_pokemon);
    trainer_thread_context -> execution_function = (void (*)(void *)) catch_action_execution_function;

    execute_trainer_thread_context(trainer_thread_context);
}