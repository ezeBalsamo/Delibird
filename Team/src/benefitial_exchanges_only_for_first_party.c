#include <waiting_actions.h>
#include <identified_exchanges_provider.h>
#include "benefitial_exchanges_only_for_first_party.h"

t_exchange_inferrer* benefitial_only_for_first_party;

bool benefitial_only_for_first_party_can_handle_function(t_list* benefitial_exchanges_for_first, t_list* benefitial_exchanges_for_second) {
    return !list_is_empty(benefitial_exchanges_for_first) && list_is_empty(benefitial_exchanges_for_second);
}

t_list* benefitial_only_for_first_party_infer_function(t_trainer_thread_context* trainer_thread_context,
                                                       t_list* benefitial_exchanges_for_first,
                                                       t_trainer_thread_context* another_trainer_thread_context,
                                                       t_list* benefitial_exchanges_for_second){

    (void) benefitial_exchanges_for_second;

    t_list* exchanges = list_create();
    t_waiting_for_exchange_action* waiting_for_exchange_action = internal_thread_action_in(trainer_thread_context);

    for(int i = 0; i < list_size(benefitial_exchanges_for_first); i++){
        char* first_party_pokemon_name = list_get(waiting_for_exchange_action -> pokemons_in_excess, i);
        char* second_party_pokemon_name = list_get(benefitial_exchanges_for_first, i);

        t_identified_exchange* identified_exchange =
                identified_exchange_for(BENEFITIAL_ONLY_FOR_FIRST_PARTY,
                                        first_party_pokemon_name,
                                        trainer_thread_context,
                                        second_party_pokemon_name,
                                        another_trainer_thread_context);

        list_add(exchanges, identified_exchange);
    }

    return exchanges;
}

void initialize_benefitial_exchanges_only_for_first_party(){
    benefitial_only_for_first_party = safe_malloc(sizeof(t_exchange_inferrer));
    benefitial_only_for_first_party -> can_handle_function = benefitial_only_for_first_party_can_handle_function;
    benefitial_only_for_first_party -> infer_function = benefitial_only_for_first_party_infer_function;
}

t_exchange_inferrer* benefitial_exchanges_only_for_first_party(){
    return benefitial_only_for_first_party;
}