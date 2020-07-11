#include <commons/collections/list.h>
#include <dispatcher_queues.h>
#include <waiting_actions.h>
#include <commons/string.h>
#include <team_logs_manager.h>
#include <trades_calculator.h>
#include "deadlock_detecter.h"
#include "../../Utils/include/t_list_extension.h"

bool is_dispatcher_queue_empty(t_dispatcher_queue* dispatcher_queue){
    bool is_empty;

    void _is_empty(){
        is_empty = list_is_empty(dispatcher_queue -> trainer_thread_contexts);
    }

    handling_concurrency_do(dispatcher_queue, _is_empty);
    return is_empty;
}

bool is_waiting_for_trade(t_trainer_thread_context* trainer_thread_context){
    return internal_thread_action_type_in(trainer_thread_context) == WAITING_FOR_TRADE;
}

bool are_all_blocked_trainer_thread_contexts_waiting_for_trade(){

    t_dispatcher_queue* dispatcher_queue = dispatcher_queue_of(BLOCKED);
    bool are_all_waiting_for_trade;

    void _are_all_trainer_thread_contexts_waiting_for_trade(){
        are_all_waiting_for_trade =
                list_all_satisfy(dispatcher_queue -> trainer_thread_contexts,
                                 (bool (*)(void *)) is_waiting_for_trade);
    }

    handling_concurrency_do(dispatcher_queue, _are_all_trainer_thread_contexts_waiting_for_trade);
    return are_all_waiting_for_trade;
}

bool deadlock_exists(){

    t_list* queues_expected_to_be_empty = list_create();
    list_add(queues_expected_to_be_empty, dispatcher_queue_of(NEW));
    list_add(queues_expected_to_be_empty, dispatcher_queue_of(READY));
    list_add(queues_expected_to_be_empty, dispatcher_queue_of(EXECUTE));

    bool deadlock_has_occurred =
            list_all_satisfy(queues_expected_to_be_empty, (bool (*)(void *)) is_dispatcher_queue_empty) &&
            are_all_blocked_trainer_thread_contexts_waiting_for_trade();

    list_destroy(queues_expected_to_be_empty);
    return deadlock_has_occurred;
}

t_list* trainer_thread_contexts_having_in_excess(t_list* trainer_thread_contexts, char* pokemon_name){

    bool _has_in_excess(t_trainer_thread_context* trainer_thread_context){
        t_waiting_for_trade_action* waiting_for_trade_action = internal_thread_action_in(trainer_thread_context);

        return list_includes(waiting_for_trade_action -> pokemons_in_excess,
                             pokemon_name,
                             (bool (*)(void *, void *)) string_equals_ignore_case);
    }

    return list_filter(trainer_thread_contexts, (bool (*)(void *)) _has_in_excess);
}

bool is_benefitial_for_both_parties(t_identified_trade* identified_trade){
    return identified_trade -> trade_type == BENEFITIAL_FOR_BOTH_PARTIES;
}

t_list* trainer_thread_contexts_with_benefitial_trade_for_both_parties_for(
        t_trainer_thread_context* trainer_thread_context_to_compare,
        t_list* trainer_thread_contexts, char* pokemon_name){

    bool _has_benefitial_trade_for_both_parties(t_trainer_thread_context* trainer_thread_context){
        t_list* identified_trades = trades_between(trainer_thread_context_to_compare, trainer_thread_context);
        bool is_benefitial_for_both_trades = list_any_satisfy(identified_trades, is_benefitial_for_both_parties);
        list_destroy(identified_trades);
        return is_benefitial_for_both_trades;
    }

    return list_filter(trainer_thread_contexts, (bool (*)(void *)) _has_benefitial_trade_for_both_parties);

}

void calculate_and_notify_deadlocks_quantity(){
    t_list* trainer_thread_contexts = trainer_thread_contexts_in(BLOCKED);
    t_list* duplicated_trainer_thread_contexts = list_duplicate(trainer_thread_contexts);

    for(int i = 0; i < list_size(trainer_thread_contexts); i++){

        t_trainer_thread_context* trainer_thread_context = list_remove_first(duplicated_trainer_thread_contexts);
        t_waiting_for_trade_action* waiting_for_trade_action = internal_thread_action_in(trainer_thread_context);

        t_list* duplicated_required_pokemons_not_caught =
                list_duplicate(waiting_for_trade_action -> required_pokemons_not_caught);

        for(int j = 0; j < list_size(waiting_for_trade_action -> required_pokemons_not_caught); j++){

            char* pokemon_name = list_remove_first(duplicated_required_pokemons_not_caught);

            t_list* filtered_trainer_thread_contexts =
                    trainer_thread_contexts_having_in_excess(duplicated_trainer_thread_contexts, pokemon_name);

            if(list_is_empty(filtered_trainer_thread_contexts)){
                log_pokemon_not_found_error_for(pokemon_name);
            }

            t_list* trainer_thread_context_with_benefitial_for_both_parties_trade =
                    trainer_thread_contexts_with_benefitial_trade_for_both_parties_for(
                            trainer_thread_context, filtered_trainer_thread_contexts, pokemon_name);
        }


    }
}