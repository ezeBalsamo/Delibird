#ifndef DELIBIRD_MAP_H
#define DELIBIRD_MAP_H

#include "team_manager.h"

void initialize_map();
void load_pokemon_in_map(t_localizable_object* localized_object);
void remove_pokemon_from_matrix(t_localizable_object* t_localizable_object);

#endif //DELIBIRD_MAP_H
