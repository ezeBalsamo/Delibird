#ifndef DELIBIRD_WAITING_ACTIONS_H
#define DELIBIRD_WAITING_ACTIONS_H

#include "trainer_threads.h"
#include "catch_action.h"

typedef struct Waiting_catch_response_action{
    t_localizable_object* localizable_pokemon;
    int message_id;
    bool caught_succeeded;
}t_waiting_catch_response_action;

typedef struct Waiting_for_trade_action{
    t_list* pokemons_in_excess;
    t_list* required_pokemons_not_caught;
}t_waiting_for_trade_action;

t_thread_action* waiting_catch_response_thread_action_for(t_localizable_object* localizable_pokemon, int message_id);
t_thread_action* waiting_for_more_pokemons_thread_action();
t_thread_action* waiting_for_trade_thread_action_for(t_trainer* trainer);

#endif //DELIBIRD_WAITING_ACTIONS_H
