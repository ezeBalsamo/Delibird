#ifndef DELIBIRD_GOAL_CALCULATOR_H
#define DELIBIRD_GOAL_CALCULATOR_H

#include <commons/collections/list.h>
#include "team_manager.h"

t_list* team_global_goal_according_to(t_list* trainers);
t_list* requirements_of(t_trainer* trainer);
bool can_catch_pokemons(t_trainer* trainer);

#endif //DELIBIRD_GOAL_CALCULATOR_H
