#ifndef DELIBIRD_MOVEMENT_ACTION_H
#define DELIBIRD_MOVEMENT_ACTION_H

#include "team_manager.h"
#include "pokemon_occurrences.h"
#include "trainer_threads.h"

typedef struct Movement_action{
    t_localizable_object* localizable_trainer;
    t_localizable_object* destiny_object;
}t_movement_action;

t_thread_action* movement_thread_action_for(t_localizable_object* localizable_trainer, t_localizable_object* destiny_object);

void movement_action_execution_function(t_trainer_thread_context* trainer_thread_context);

#endif //DELIBIRD_MOVEMENT_ACTION_H
