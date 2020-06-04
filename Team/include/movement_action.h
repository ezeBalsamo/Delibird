#ifndef DELIBIRD_MOVEMENT_ACTION_H
#define DELIBIRD_MOVEMENT_ACTION_H

#include "team_manager.h"
#include "map.h"
#include "trainer_threads.h"

typedef struct Movement_action{
    char* action_type_description;
    t_localizable_object* localizable_trainer;
    t_localizable_object* destiny_object;
}t_movement_action;

t_movement_action* movement_action_for(t_localizable_object* localizable_trainer, t_localizable_object* destiny_object);

void movement_action_execution_function(t_trainer_thread_context* trainer_thread_context);

#endif //DELIBIRD_MOVEMENT_ACTION_H
