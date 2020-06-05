#ifndef DELIBIRD_DISTANCE_CALCULATOR_H
#define DELIBIRD_DISTANCE_CALCULATOR_H

#include "team_manager.h"
#include "trainer_threads.h"

t_trainer_thread_context* trainer_thread_context_closest_to(t_list* trainer_thread_contexts, t_localizable_object* localizable_pokemon);
t_localizable_object* closest_pokemon_to(t_list* localizable_pokemons, t_localizable_object* localizable_trainer);

#endif //DELIBIRD_DISTANCE_CALCULATOR_H
