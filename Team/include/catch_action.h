#ifndef DELIBIRD_CATCH_ACTION_H
#define DELIBIRD_CATCH_ACTION_H

#include "team_manager.h"
#include "trainer_threads.h"

typedef struct Catch_action{
    t_localizable_object* localizable_pokemon;
}t_catch_action;

t_thread_action* catch_thread_action_for(t_localizable_object* localizable_pokemon);

void catch_action_execution_function(t_trainer_thread_context* trainer_thread_context);

#endif //DELIBIRD_CATCH_ACTION_H
