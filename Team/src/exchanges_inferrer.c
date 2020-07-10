#include "exchanges_inferrer.h"
#include "../../Utils/include/garbage_collector.h"
#include <stdlib.h>
#include <benefitial_exchanges_for_both_parties.h>
#include <benefitial_exchanges_only_for_first_party.h>
#include <benefitial_exchanges_only_for_second_party.h>
#include <team_logs_manager.h>
#include <non_benefitial_exchange.h>

t_list* exchanges_inferrers;

void initialize_exchanges_inferrer(){
    exchanges_inferrers = list_create();

    initialize_benefitial_exchanges_for_both_parties();
    initialize_benefitial_exchanges_only_for_first_party();
    initialize_benefitial_exchanges_only_for_second_party();
    initialize_non_benefitial_exchange();

    list_add(exchanges_inferrers, benefitial_exchanges_for_both_parties());
    list_add(exchanges_inferrers, benefitial_exchanges_only_for_first_party());
    list_add(exchanges_inferrers, benefitial_exchanges_only_for_second_party());
    list_add(exchanges_inferrers, non_benefitial_exchange());
}

t_list* infer_exchanges(t_trainer_thread_context* trainer_thread_context,
                        t_list* benefitial_exchanges_for_first,
                        t_trainer_thread_context* another_trainer_thread_context,
                        t_list* benefitial_exchanges_for_second){

    bool _can_handle(t_exchange_inferrer* exchange_inferrer){
        return exchange_inferrer -> can_handle_function (benefitial_exchanges_for_first, benefitial_exchanges_for_second);
    }

    t_exchange_inferrer* exchange_inferrer_found = list_find(exchanges_inferrers, (bool (*)(void *)) _can_handle);

    if(!exchange_inferrer_found){
        log_no_suitable_exchange_inferrer_found_error_for(trainer_thread_context, another_trainer_thread_context);
        free_system();
    }

    return exchange_inferrer_found -> infer_function(trainer_thread_context, benefitial_exchanges_for_first,
                                                     another_trainer_thread_context, benefitial_exchanges_for_second);
}

void free_exchanges_inferrer(){
    list_destroy_and_destroy_elements(exchanges_inferrers, free);
}