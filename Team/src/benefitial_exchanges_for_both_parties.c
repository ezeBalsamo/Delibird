#include <identified_exchanges_provider.h>
#include <waiting_actions.h>
#include <commons/string.h>
#include "benefitial_exchanges_for_both_parties.h"
#include "../../Utils/include/t_list_extension.h"

t_exchange_inferrer* benefitial_for_both_parties;

bool benefitial_for_both_parties_can_handle_function(t_list* benefitial_exchanges_for_first, t_list* benefitial_exchanges_for_second){
    return !list_is_empty(benefitial_exchanges_for_first) && !list_is_empty(benefitial_exchanges_for_second);
}

t_list* completed_exchanges_for_party(t_trainer_thread_context* trainer_thread_context,
                                      t_list* benefitial_exchanges_for_party,
                                      t_list* benefitial_exchanges_for_another_party){

    t_list* completed_exchanges_for_party = list_duplicate(benefitial_exchanges_for_party);

    int benefitial_exchanges_for_first_quantity = list_size(benefitial_exchanges_for_party);
    int benefitial_exchanges_for_second_quantity = list_size(benefitial_exchanges_for_another_party);

    if(benefitial_exchanges_for_first_quantity < benefitial_exchanges_for_second_quantity){

        t_waiting_for_exchange_action* waiting_for_exchange_action = internal_thread_action_in(trainer_thread_context);
        int one_sided_benefitial_exchanges_amount = benefitial_exchanges_for_second_quantity - benefitial_exchanges_for_first_quantity;

        t_list* remaining_pokemons_in_excess =
                list_difference(waiting_for_exchange_action -> pokemons_in_excess,
                                benefitial_exchanges_for_party,
                                (bool (*)(void *, void *)) string_equals_ignore_case);

        for(int i = 0; i < one_sided_benefitial_exchanges_amount; i++){
            char* pokemon_to_exchange = list_get(remaining_pokemons_in_excess, i);
            list_add(completed_exchanges_for_party, pokemon_to_exchange);
        }

        list_destroy(remaining_pokemons_in_excess);
    }

    return completed_exchanges_for_party;
}

t_list* benefitial_for_both_parties_infer_function(t_trainer_thread_context* first_party_trainer_thread_context,
                                                   t_list* benefitial_exchanges_for_first,
                                                   t_trainer_thread_context* second_party_trainer_thread_context,
                                                   t_list* benefitial_exchanges_for_second){

    t_list* exchanges = list_create();

    t_list* exchanges_for_first =
            completed_exchanges_for_party(second_party_trainer_thread_context,
                                          benefitial_exchanges_for_first,
                                          benefitial_exchanges_for_second);

    t_list* exchanges_for_second =
            completed_exchanges_for_party(first_party_trainer_thread_context,
                                          benefitial_exchanges_for_second,
                                          benefitial_exchanges_for_first);

    for(int i = 0; i < list_size(exchanges_for_first); i++){
        char* first_party_pokemon_name = list_get(exchanges_for_second, i);
        char* second_party_pokemon_name = list_get(exchanges_for_first, i);

        t_identified_exchange* identified_exchange =
                identified_exchange_for(BENEFITIAL_FOR_BOTH_PARTIES,
                                        first_party_pokemon_name,
                                        first_party_trainer_thread_context,
                                        second_party_pokemon_name,
                                        second_party_trainer_thread_context);

        list_add(exchanges, identified_exchange);
    }

    list_destroy(exchanges_for_first);
    list_destroy(exchanges_for_second);

    return exchanges;
}

void initialize_benefitial_exchanges_for_both_parties(){
    benefitial_for_both_parties = safe_malloc(sizeof(t_exchange_inferrer));
    benefitial_for_both_parties -> can_handle_function = benefitial_for_both_parties_can_handle_function;
    benefitial_for_both_parties -> infer_function = benefitial_for_both_parties_infer_function;
}

t_exchange_inferrer* benefitial_exchanges_for_both_parties(){
    return benefitial_for_both_parties;
}