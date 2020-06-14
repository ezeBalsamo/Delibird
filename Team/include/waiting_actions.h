#ifndef DELIBIRD_WAITING_ACTIONS_H
#define DELIBIRD_WAITING_ACTIONS_H

#include "trainer_threads.h"
#include "catch_action.h"

typedef struct Waiting_catch_response_action{
    t_catch_action* catch_action;
    int message_id;
}t_waiting_catch_response_action;

t_thread_action* waiting_catch_response_thread_action_for(t_catch_action* catch_action, int message_id);
t_thread_action* waiting_for_more_pokemons_thread_action();

#endif //DELIBIRD_WAITING_ACTIONS_H
