#ifndef DELIBIRD_IDENTIFIED_EXCHANGES_PROVIDER_H
#define DELIBIRD_IDENTIFIED_EXCHANGES_PROVIDER_H

#include <stdint.h>
#include "trainer_threads.h"

enum Exchange_types{
    BENEFITIAL_FOR_BOTH_PARTIES,
    BENEFITIAL_ONLY_FOR_FIRST_PARTY,
    BENEFITIAL_ONLY_FOR_SECOND_PARTY
};

typedef struct Exchange{
    char* first_party_pokemon_name;
    t_trainer_thread_context* trainer_thread_context;
    char* second_party_pokemon_name;
    t_trainer_thread_context* another_trainer_thread_context;
}t_exchange;

typedef struct Identified_exchange{
    uint32_t exchange_type;
    t_exchange* exchange;
}t_identified_exchange;

t_localizable_object* first_party_localizable_trainer_in(t_identified_exchange* identified_exchange);
t_localizable_object* second_party_localizable_trainer_in(t_identified_exchange* identified_exchange);

char* first_party_pokemon_name_in(t_identified_exchange* identified_exchange);
char* second_party_pokemon_name_in(t_identified_exchange* identified_exchange);

t_identified_exchange* identified_exchange_for(uint32_t exchange_type,
                                               char* first_party_pokemon_name,
                                               t_trainer_thread_context* trainer_thread_context,
                                               char* second_party_pokemon_name,
                                               t_trainer_thread_context* another_trainer_thread_context);

void free_identified_exchange(t_identified_exchange* identified_exchange);

#endif //DELIBIRD_IDENTIFIED_EXCHANGES_PROVIDER_H
