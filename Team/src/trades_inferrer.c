#include "trades_inferrer.h"
#include "../../Utils/include/garbage_collector.h"
#include <stdlib.h>
#include <benefitial_trades_for_both_parties.h>
#include <benefitial_trades_only_for_first_party.h>
#include <benefitial_trades_only_for_second_party.h>
#include <team_logs_manager.h>
#include <non_benefitial_trade.h>

t_list* trades_inferrers;

void initialize_trades_inferrer(){
    trades_inferrers = list_create();

    initialize_benefitial_trades_for_both_parties();
    initialize_benefitial_trades_only_for_first_party();
    initialize_benefitial_trades_only_for_second_party();
    initialize_non_benefitial_trade();

    list_add(trades_inferrers, benefitial_trades_for_both_parties());
    list_add(trades_inferrers, benefitial_trades_only_for_first_party());
    list_add(trades_inferrers, benefitial_trades_only_for_second_party());
    list_add(trades_inferrers, non_benefitial_trade());
}

t_list* infer_trades(t_trainer_thread_context* trainer_thread_context,
                     t_list* benefitial_trades_for_first,
                     t_trainer_thread_context* another_trainer_thread_context,
                     t_list* benefitial_trades_for_second){

    bool _can_handle(t_trade_inferrer* trade_inferrer){
        return trade_inferrer -> can_handle_function (benefitial_trades_for_first, benefitial_trades_for_second);
    }

    t_trade_inferrer* trade_inferrer_found = list_find(trades_inferrers, (bool (*)(void *)) _can_handle);

    if(!trade_inferrer_found){
        log_no_suitable_trade_inferrer_found_error_for(trainer_thread_context, another_trainer_thread_context);
        free_system();
    }

    return trade_inferrer_found -> infer_function(trainer_thread_context, benefitial_trades_for_first,
                                                     another_trainer_thread_context, benefitial_trades_for_second);
}

void free_trades_inferrer(){
    list_destroy_and_destroy_elements(trades_inferrers, free);
}