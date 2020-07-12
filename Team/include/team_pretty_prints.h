#ifndef DELIBIRD_TEAM_PRETTY_PRINTS_H
#define DELIBIRD_TEAM_PRETTY_PRINTS_H

#include "team_manager.h"
#include <identified_trades_provider.h>

char* localizable_pokemon_as_string(t_localizable_object* localizable_pokemon);
char* trainer_as_string(t_trainer* trainer);
char* localizable_trainer_as_string(t_localizable_object* localizable_trainer);
char* localizable_object_as_string(t_localizable_object* localizable_object);
char* state_as_string(uint32_t state);
char* pokemon_names_as_string(t_list* pokemon_names);
char* trade_to_do_as_string(t_identified_trade* identified_trade);
char* trade_completed_as_string(t_identified_trade* identified_trade);

#endif //DELIBIRD_TEAM_PRETTY_PRINTS_H
