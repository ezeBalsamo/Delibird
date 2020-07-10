#include "non_benefitial_exchange.h"

t_exchange_inferrer* non_benefitial;

bool non_benefitial_can_handle_function(t_list* benefitial_exchanges_for_first, t_list* benefitial_exchanges_for_second){
    return list_is_empty(benefitial_exchanges_for_first) && list_is_empty(benefitial_exchanges_for_second);
}

t_list* non_benefitial_infer_function(t_trainer_thread_context* trainer_thread_context,
                                      t_list* benefitial_exchanges_for_first,
                                      t_trainer_thread_context* another_trainer_thread_context,
                                      t_list* benefitial_exchanges_for_second){

    (void) trainer_thread_context;
    (void) benefitial_exchanges_for_first;
    (void) another_trainer_thread_context;
    (void) benefitial_exchanges_for_second;

    return list_create();
}

void initialize_non_benefitial_exchange(){
    non_benefitial = safe_malloc(sizeof(t_exchange_inferrer));
    non_benefitial -> can_handle_function = non_benefitial_can_handle_function;
    non_benefitial -> infer_function = non_benefitial_infer_function;
}

t_exchange_inferrer* non_benefitial_exchange(){
    return non_benefitial;
}