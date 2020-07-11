#include "non_benefitial_trade.h"

t_trade_inferrer* non_benefitial;

bool non_benefitial_can_handle_function(t_list* benefitial_trades_for_first, t_list* benefitial_trades_for_second){
    return list_is_empty(benefitial_trades_for_first) && list_is_empty(benefitial_trades_for_second);
}

t_list* non_benefitial_infer_function(t_trainer_thread_context* trainer_thread_context,
                                      t_list* benefitial_trades_for_first,
                                      t_trainer_thread_context* another_trainer_thread_context,
                                      t_list* benefitial_trades_for_second){

    (void) trainer_thread_context;
    (void) benefitial_trades_for_first;
    (void) another_trainer_thread_context;
    (void) benefitial_trades_for_second;

    return list_create();
}

void initialize_non_benefitial_trade(){
    non_benefitial = safe_malloc(sizeof(t_trade_inferrer));
    non_benefitial -> can_handle_function = non_benefitial_can_handle_function;
    non_benefitial -> infer_function = non_benefitial_infer_function;
}

t_trade_inferrer* non_benefitial_trade(){
    return non_benefitial;
}