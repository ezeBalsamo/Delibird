#include "exchange_action.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/chained_evaluation.h"
#include <stdlib.h>
#include <deadlock_solver.h>
#include <identified_exchanges_provider.h>
#include <scheduling_algorithm.h>
#include <team_logs_manager.h>
#include <trainer_thread_context_state_chained_evaluation.h>
#include <team_configuration_manager.h>
#include <unistd.h>
#include <trainer_thread_context_execution_cycle.h>
#include <event_notifier.h>
#include <team_pretty_prints.h>

extern sem_t exchange_resolution_semaphore;
int deadlock_consume_cycle_quantity = 5;

void times_repeat(int times, void (*function) ()){
    for(int i = 0; i < times; i++){
        function();
    }
}

void exchange_with(t_identified_exchange* identified_exchange){

    t_localizable_object* first_party_localizable_trainer =
            first_party_localizable_trainer_in(identified_exchange);
    char* first_party_pokemon_name = first_party_pokemon_name_in(identified_exchange);

    t_localizable_object* second_party_localizable_trainer =
            second_party_localizable_trainer_in(identified_exchange);
    char* second_party_pokemon_name = second_party_pokemon_name_in(identified_exchange);

    exchange_trainers_pokemons(first_party_pokemon_name,
                               first_party_localizable_trainer,
                               second_party_pokemon_name,
                               second_party_localizable_trainer);
}

void notify_and_log_exchange_realized_according_to(t_identified_exchange* identified_exchange){
    char* printable_exchange_completed = exchange_completed_as_string(identified_exchange);
    log_exchange_completed(printable_exchange_completed);
    notify_with_argument(PRODUCED_AND_SOLVED_DEADLOCK, printable_exchange_completed);
}

void exchange_action_execution_function(t_trainer_thread_context* trainer_thread_context){

    t_list* identified_exchanges = current_identified_exchanges_in_process();

    void _exchange(t_identified_exchange* identified_exchange){
        log_exchange_to_realize_according_to(identified_exchange);

        void _notify(){
            notify_with_argument(EXECUTION_CYCLE_CONSUMED_BY_TRAINER,
                                 trainer_from_thread_context(trainer_thread_context));
        }

        times_repeat(deadlock_consume_cycle_quantity, _notify);

        exchange_with(identified_exchange);
        notify_and_log_exchange_realized_according_to(identified_exchange);
    }

    list_iterate(identified_exchanges, (void (*)(void *)) _exchange);
    exchange_action_completed_using(identified_exchanges);
    sem_post(&exchange_resolution_semaphore);
}

t_thread_action* exchange_thread_action(){

    t_thread_action* thread_action = new_thread_action();
    thread_action -> request -> operation = EXCHANGE;
    thread_action -> request -> structure = NULL;
    thread_action -> request -> sanitizer_function = free;
    thread_action -> execution_function = (void (*)(void *)) exchange_action_execution_function;
}