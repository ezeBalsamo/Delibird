#include "trade_action.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/chained_evaluation.h"
#include <stdlib.h>
#include <deadlock_solver.h>
#include <identified_trades_provider.h>
#include <scheduling_algorithm.h>
#include <team_logs_manager.h>
#include <trainer_thread_context_state_chained_evaluation.h>
#include <team_configuration_manager.h>
#include <unistd.h>
#include <trainer_thread_context_execution_cycle.h>
#include <event_notifier.h>
#include <team_pretty_prints.h>

extern sem_t trade_resolution_semaphore;
int deadlock_consume_cycle_quantity = 5;

void times_repeat(int times, void (*function) ()){
    for(int i = 0; i < times; i++){
        function();
    }
}

void trade_with(t_identified_trade* identified_trade){

    t_localizable_object* first_party_localizable_trainer =
            first_party_localizable_trainer_in(identified_trade);
    char* first_party_pokemon_name = first_party_pokemon_name_in(identified_trade);

    t_localizable_object* second_party_localizable_trainer =
            second_party_localizable_trainer_in(identified_trade);
    char* second_party_pokemon_name = second_party_pokemon_name_in(identified_trade);

    trade_trainers_pokemons(first_party_pokemon_name,
                               first_party_localizable_trainer,
                               second_party_pokemon_name,
                               second_party_localizable_trainer);
}

void notify_and_log_trade_realized_according_to(t_identified_trade* identified_trade){
    char* printable_trade_completed = trade_completed_as_string(identified_trade);
    log_trade_completed(printable_trade_completed);
    notify_with_argument(TRADE_COMPLETED, printable_trade_completed);
}

void trade_action_execution_function(t_trainer_thread_context* trainer_thread_context){

    t_list* identified_trades = current_identified_trades_in_process();

    void _trade(t_identified_trade* identified_trade){
        log_trade_to_do_according_to(identified_trade);

        void _notify(){
            notify_with_argument(EXECUTION_CYCLE_CONSUMED_BY_TRAINER,
                                 trainer_from_thread_context(trainer_thread_context));
        }

        times_repeat(deadlock_consume_cycle_quantity, _notify);

        trade_with(identified_trade);
        notify_and_log_trade_realized_according_to(identified_trade);
    }

    list_iterate(identified_trades, (void (*)(void *)) _trade);
    trade_action_completed_using(identified_trades);
    sem_post(&trade_resolution_semaphore);
}

t_thread_action* trade_thread_action(){

    t_thread_action* thread_action = new_thread_action();
    thread_action -> request -> operation = TRADE;
    thread_action -> request -> structure = NULL;
    thread_action -> request -> sanitizer_function = free;
    thread_action -> execution_function = (void (*)(void *)) trade_action_execution_function;

    return thread_action;
}