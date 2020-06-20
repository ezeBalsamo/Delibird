#ifndef DELIBIRD_POKEMON_OCCURRENCES_H
#define DELIBIRD_POKEMON_OCCURRENCES_H

#include "team_manager.h"

void initialize_pokemon_occurrences();

void new_occurrence_of(t_targetable_object* targetable_pokemon);
void remove_occurrence_of(t_localizable_object* localizable_pokemon);
uint32_t occurrences_of_pokemon_named(char* pokemon_name);
t_list* not_yet_targeted_pokemons();

void free_pokemon_occurrences();

#endif //DELIBIRD_POKEMON_OCCURRENCES_H
