#ifndef DELIBIRD_MAP_H
#define DELIBIRD_MAP_H

#include "team_manager.h"

void initialize_map();
void load_pokemon_in_map(t_targetable_object* targetable_pokemon);
void remove_pokemon_from_map(t_localizable_object* localizable_pokeon);
uint32_t occurrences_amount_in_map_of(char* pokemon_name);
t_list* not_yet_targeted_pokemons();

#endif //DELIBIRD_MAP_H
