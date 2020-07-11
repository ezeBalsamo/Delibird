#include <identified_trades_provider.h>
#include <waiting_actions.h>
#include <commons/string.h>
#include "benefitial_trades_for_both_parties.h"
#include "../../Utils/include/t_list_extension.h"

t_trade_inferrer* benefitial_for_both_parties;

bool benefitial_for_both_parties_can_handle_function(t_list* benefitial_trades_for_first, t_list* benefitial_trades_for_second){
    return !list_is_empty(benefitial_trades_for_first) && !list_is_empty(benefitial_trades_for_second);
}

t_list* remaining_benefitial_trades_for_one_party(int trade_type,
                                                  t_trainer_thread_context* party_trainer_thread_context,
                                                  t_trainer_thread_context* another_party_trainer_thread_context,
                                                  t_list* remaining_benefitial_trades_for_party,
                                                  t_list* benefitial_trades_for_another_party){

    t_list* remaining_trades = list_create();

    t_waiting_for_trade_action* waiting_for_trade_action = internal_thread_action_in(party_trainer_thread_context);
    int one_sided_benefitial_trades_amount = list_size(remaining_benefitial_trades_for_party);

    t_list* remaining_pokemons_in_excess =
            list_difference(waiting_for_trade_action -> pokemons_in_excess,
                            benefitial_trades_for_another_party,
                            (bool (*)(void *, void *)) string_equals_ignore_case);

    for(int i = 0; i < one_sided_benefitial_trades_amount; i++){
        char* party_pokemon_name = list_remove_first(remaining_pokemons_in_excess);
        char* another_party_pokemon_name = list_remove_first(remaining_benefitial_trades_for_party);

        t_identified_trade* identified_trade =
                identified_trade_for(trade_type,
                                     party_pokemon_name,
                                     party_trainer_thread_context,
                                     another_party_pokemon_name,
                                     another_party_trainer_thread_context);

        list_add(remaining_trades, identified_trade);
    }

    list_destroy(remaining_pokemons_in_excess);

    return remaining_trades;
}

t_list* benefitial_for_both_parties_infer_function(t_trainer_thread_context* first_party_trainer_thread_context,
                                                   t_list* benefitial_trades_for_first,
                                                   t_trainer_thread_context* second_party_trainer_thread_context,
                                                   t_list* benefitial_trades_for_second){

    t_list* trades = list_create();

    t_list* duplicated_trades_for_first = list_duplicate(benefitial_trades_for_first);
    t_list* duplicated_trades_for_second = list_duplicate(benefitial_trades_for_second);

    int minimum = minimum_integer_between(list_size(benefitial_trades_for_first), list_size(benefitial_trades_for_second));

    for(int i = 0; i < minimum; i++){
        char* first_party_pokemon_name = list_remove_first(duplicated_trades_for_second);
        char* second_party_pokemon_name = list_remove_first(duplicated_trades_for_first);

        t_identified_trade* identified_trade =
                identified_trade_for(BENEFITIAL_FOR_BOTH_PARTIES,
                                        first_party_pokemon_name,
                                        first_party_trainer_thread_context,
                                        second_party_pokemon_name,
                                        second_party_trainer_thread_context);

        list_add(trades, identified_trade);
    }

    if(list_is_empty(duplicated_trades_for_first)){

        t_list* remaining_trades =
                remaining_benefitial_trades_for_one_party(BENEFITIAL_ONLY_FOR_SECOND_PARTY,
                                                          second_party_trainer_thread_context,
                                                          first_party_trainer_thread_context,
                                                          duplicated_trades_for_second,
                                                          benefitial_trades_for_first);

        list_add_all(trades, remaining_trades);
        list_destroy(remaining_trades);
    }

    if(list_is_empty(duplicated_trades_for_second)){

        t_list* remaining_trades =
                remaining_benefitial_trades_for_one_party(BENEFITIAL_ONLY_FOR_FIRST_PARTY,
                                                          first_party_trainer_thread_context,
                                                          second_party_trainer_thread_context,
                                                          duplicated_trades_for_first,
                                                          benefitial_trades_for_second);

        list_add_all(trades, remaining_trades);
        list_destroy(remaining_trades);
    }

    list_destroy(duplicated_trades_for_first);
    list_destroy(duplicated_trades_for_second);

    return trades;
}

void initialize_benefitial_trades_for_both_parties(){
    benefitial_for_both_parties = safe_malloc(sizeof(t_trade_inferrer));
    benefitial_for_both_parties -> can_handle_function = benefitial_for_both_parties_can_handle_function;
    benefitial_for_both_parties -> infer_function = benefitial_for_both_parties_infer_function;
}

t_trade_inferrer* benefitial_trades_for_both_parties(){
    return benefitial_for_both_parties;
}