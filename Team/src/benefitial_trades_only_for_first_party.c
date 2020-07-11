#include <waiting_actions.h>
#include <identified_trades_provider.h>
#include "benefitial_trades_only_for_first_party.h"

t_trade_inferrer* benefitial_only_for_first_party;

bool benefitial_only_for_first_party_can_handle_function(t_list* benefitial_trades_for_first, t_list* benefitial_trades_for_second) {
    return !list_is_empty(benefitial_trades_for_first) && list_is_empty(benefitial_trades_for_second);
}

t_list* benefitial_only_for_first_party_infer_function(t_trainer_thread_context* first_party_trainer_thread_context,
                                                       t_list* benefitial_trades_for_first,
                                                       t_trainer_thread_context* second_party_trainer_thread_context,
                                                       t_list* benefitial_trades_for_second){

    (void) benefitial_trades_for_second;

    t_list* trades = list_create();
    t_waiting_for_trade_action* waiting_for_trade_action = internal_thread_action_in(first_party_trainer_thread_context);

    for(int i = 0; i < list_size(benefitial_trades_for_first); i++){
        char* first_party_pokemon_name = list_get(waiting_for_trade_action -> pokemons_in_excess, i);
        char* second_party_pokemon_name = list_get(benefitial_trades_for_first, i);

        t_identified_trade* identified_trade =
                identified_trade_for(BENEFITIAL_ONLY_FOR_FIRST_PARTY,
                                     first_party_pokemon_name,
                                     first_party_trainer_thread_context,
                                     second_party_pokemon_name,
                                     second_party_trainer_thread_context);

        list_add(trades, identified_trade);
    }

    return trades;
}

void initialize_benefitial_trades_only_for_first_party(){
    benefitial_only_for_first_party = safe_malloc(sizeof(t_trade_inferrer));
    benefitial_only_for_first_party -> can_handle_function = benefitial_only_for_first_party_can_handle_function;
    benefitial_only_for_first_party -> infer_function = benefitial_only_for_first_party_infer_function;
}

t_trade_inferrer* benefitial_trades_only_for_first_party(){
    return benefitial_only_for_first_party;
}