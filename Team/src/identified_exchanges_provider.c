#include "identified_exchanges_provider.h"
#include <stdlib.h>

t_localizable_object* first_party_localizable_trainer_in(t_identified_exchange* identified_exchange){
    return identified_exchange -> exchange -> first_party_trainer_thread_context -> localizable_trainer;
}

t_localizable_object* second_party_localizable_trainer_in(t_identified_exchange* identified_exchange){
    return identified_exchange -> exchange -> second_party_trainer_thread_context -> localizable_trainer;
}

char* first_party_pokemon_name_in(t_identified_exchange* identified_exchange){
    return identified_exchange -> exchange -> first_party_pokemon_name;
}

char* second_party_pokemon_name_in(t_identified_exchange* identified_exchange){
    return identified_exchange -> exchange -> second_party_pokemon_name;
}

t_identified_exchange* identified_exchange_for(uint32_t exchange_type,
                                               char* first_party_pokemon_name,
                                               t_trainer_thread_context* first_party_trainer_thread_context,
                                               char* second_party_pokemon_name,
                                               t_trainer_thread_context* second_party_trainer_thread_context){

    t_exchange* exchange = safe_malloc(sizeof(t_exchange));
    exchange -> first_party_pokemon_name = first_party_pokemon_name;
    exchange -> first_party_trainer_thread_context = first_party_trainer_thread_context;
    exchange -> second_party_pokemon_name = second_party_pokemon_name;
    exchange -> second_party_trainer_thread_context = second_party_trainer_thread_context;

    t_identified_exchange* identified_exchange = safe_malloc(sizeof(t_identified_exchange));
    identified_exchange -> exchange_type = exchange_type;
    identified_exchange -> exchange = exchange;

    return identified_exchange;
}

void free_identified_exchange(t_identified_exchange* identified_exchange){
    free(identified_exchange -> exchange);
    free(identified_exchange);
}