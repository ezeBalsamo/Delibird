#include "exchange_provider.h"

t_identified_exchange* identified_exchange_for(uint32_t exchange_type,
                                               char* first_party_pokemon,
                                               t_trainer_thread_context* trainer_thread_context,
                                               char* second_party_pokemon,
                                               t_trainer_thread_context* another_trainer_thread_context){

    t_exchange* exchange = safe_malloc(sizeof(t_exchange));
    exchange -> first_party_pokemon = first_party_pokemon;
    exchange -> trainer_thread_context = trainer_thread_context;
    exchange -> second_party_pokemon = second_party_pokemon;
    exchange -> another_trainer_thread_context = another_trainer_thread_context;

    t_identified_exchange* identified_exchange = safe_malloc(sizeof(t_identified_exchange));
    identified_exchange -> exchange_type = exchange_type;
    identified_exchange -> exchange = exchange;

    return identified_exchange;
}