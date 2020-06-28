#ifndef DELIBIRD_DISTANCE_CALCULATOR_H
#define DELIBIRD_DISTANCE_CALCULATOR_H

#include "team_manager.h"
#include "trainer_threads.h"

typedef struct Distance_between_two{
    void* first_object;
    void* second_object;
    uint32_t distance;
}t_distance_between_two;

t_trainer_thread_context* trainer_thread_context_closest_to_localizable_pokemon(t_list* trainer_thread_contexts, t_localizable_object* localizable_pokemon);
t_targetable_object* targetable_pokemon_closest_to_localizable_trainer(t_list* targetable_pokemons, t_localizable_object* localizable_trainer);
t_list* closest_pair_of_trainer_thread_contexts_in(t_list* trainer_thread_contexts);

#endif //DELIBIRD_DISTANCE_CALCULATOR_H
