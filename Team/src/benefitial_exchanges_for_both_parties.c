#include <identified_exchanges_provider.h>
#include "benefitial_exchanges_for_both_parties.h"
#include "../../Utils/include/t_list_extension.h"

t_exchange_inferrer* benefitial_for_both_parties;

bool benefitial_for_both_parties_can_handle_function(t_list* benefitial_exchanges_for_first, t_list* benefitial_exchanges_for_second){
    return !list_is_empty(benefitial_exchanges_for_first) && !list_is_empty(benefitial_exchanges_for_second);
}

t_list* benefitial_for_both_parties_infer_function(t_trainer_thread_context* trainer_thread_context,
                                                    t_list* benefitial_exchanges_for_first,
                                                    t_trainer_thread_context* another_trainer_thread_context,
                                                    t_list* benefitial_exchanges_for_second){

    t_list* exchanges = list_create();
    t_list* duplicated_benefitial_exchanges_for_first = list_duplicate(benefitial_exchanges_for_first);
    t_list* duplicated_benefitial_exchanges_for_second = list_duplicate(benefitial_exchanges_for_second);

    int benefitial_exchanges_for_first_quantity = list_size(benefitial_exchanges_for_first);
    int benefitial_exchanges_for_second_quantity = list_size(benefitial_exchanges_for_second);

    int minimum = minimum_integer_between(benefitial_exchanges_for_first_quantity,
                                          benefitial_exchanges_for_second_quantity);

    for(int i = 0; i < minimum; i++){
        char* first_party_pokemon_name = list_remove_first(duplicated_benefitial_exchanges_for_second);
        char* second_party_pokemon_name = list_remove_first(duplicated_benefitial_exchanges_for_first);

        t_identified_exchange* identified_exchange =
                identified_exchange_for(BENEFITIAL_FOR_BOTH_PARTIES,
                                        first_party_pokemon_name,
                                        trainer_thread_context,
                                        second_party_pokemon_name,
                                        another_trainer_thread_context);

        list_add(exchanges, identified_exchange);
    }

    t_list* one_sided_benefitial_exchanges =
            infer_exchanges(trainer_thread_context,
                            duplicated_benefitial_exchanges_for_first,
                            another_trainer_thread_context,
                            duplicated_benefitial_exchanges_for_second);

    list_add_all(exchanges, one_sided_benefitial_exchanges);

    list_destroy(duplicated_benefitial_exchanges_for_first);
    list_destroy(duplicated_benefitial_exchanges_for_second);
    list_destroy(one_sided_benefitial_exchanges);

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