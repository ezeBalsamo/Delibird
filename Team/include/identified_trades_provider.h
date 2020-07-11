#ifndef DELIBIRD_IDENTIFIED_TRADES_PROVIDER_H
#define DELIBIRD_IDENTIFIED_TRADES_PROVIDER_H

#include <stdint.h>
#include "trainer_threads.h"

enum Trade_types{
    BENEFITIAL_FOR_BOTH_PARTIES,
    BENEFITIAL_ONLY_FOR_FIRST_PARTY,
    BENEFITIAL_ONLY_FOR_SECOND_PARTY
};

typedef struct Trade{
    char* first_party_pokemon_name;
    t_trainer_thread_context* first_party_trainer_thread_context;
    char* second_party_pokemon_name;
    t_trainer_thread_context* second_party_trainer_thread_context;
}t_trade;

typedef struct Identified_trade{
    uint32_t trade_type;
    t_trade* trade;
}t_identified_trade;

t_localizable_object* first_party_localizable_trainer_in(t_identified_trade* identified_trade);
t_localizable_object* second_party_localizable_trainer_in(t_identified_trade* identified_trade);

char* first_party_pokemon_name_in(t_identified_trade* identified_trade);
char* second_party_pokemon_name_in(t_identified_trade* identified_trade);

t_identified_trade* identified_trade_for(uint32_t trade_type,
                                         char* first_party_pokemon_name,
                                         t_trainer_thread_context* first_party_trainer_thread_context,
                                         char* second_party_pokemon_name,
                                         t_trainer_thread_context* second_party_trainer_thread_context);

void free_identified_trade(t_identified_trade* identified_trade);

#endif //DELIBIRD_IDENTIFIED_TRADES_PROVIDER_H
