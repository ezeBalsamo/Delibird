#include "identified_trades_provider.h"
#include <stdlib.h>

t_localizable_object* first_party_localizable_trainer_in(t_identified_trade* identified_trade){
    return identified_trade -> trade -> first_party_trainer_thread_context -> localizable_trainer;
}

t_localizable_object* second_party_localizable_trainer_in(t_identified_trade* identified_trade){
    return identified_trade -> trade -> second_party_trainer_thread_context -> localizable_trainer;
}

char* first_party_pokemon_name_in(t_identified_trade* identified_trade){
    return identified_trade -> trade -> first_party_pokemon_name;
}

char* second_party_pokemon_name_in(t_identified_trade* identified_trade){
    return identified_trade -> trade -> second_party_pokemon_name;
}

t_identified_trade* identified_trade_for(uint32_t trade_type,
                                         char* first_party_pokemon_name,
                                         t_trainer_thread_context* first_party_trainer_thread_context,
                                         char* second_party_pokemon_name,
                                         t_trainer_thread_context* second_party_trainer_thread_context){

    t_trade* trade = safe_malloc(sizeof(t_trade));
    trade -> first_party_pokemon_name = first_party_pokemon_name;
    trade -> first_party_trainer_thread_context = first_party_trainer_thread_context;
    trade -> second_party_pokemon_name = second_party_pokemon_name;
    trade -> second_party_trainer_thread_context = second_party_trainer_thread_context;

    t_identified_trade* identified_trade = safe_malloc(sizeof(t_identified_trade));
    identified_trade -> trade_type = trade_type;
    identified_trade -> trade = trade;

    return identified_trade;
}

void free_identified_trade(t_identified_trade* identified_trade){
    free(identified_trade -> trade);
    free(identified_trade);
}