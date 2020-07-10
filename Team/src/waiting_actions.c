#include "../include/waiting_actions.h"
#include "../../Utils/include/t_list_extension.h"
#include <stdlib.h>
#include <dispatching_reasons.h>
#include <trainer_thread_context_execution_cycle.h>
#include <pokemon_occurrences.h>
#include <team_logs_manager.h>
#include <pthread.h>
#include <goal_calculator.h>
#include <commons/string.h>

void waiting_catch_response_action_execution_function(t_trainer_thread_context* trainer_thread_context){
    t_waiting_catch_response_action* waiting_catch_response_action =
            internal_thread_action_in(trainer_thread_context);

    t_localizable_object* localizable_pokemon = waiting_catch_response_action -> localizable_pokemon;
    bool caught_succeeded = waiting_catch_response_action -> caught_succeeded;

    if(caught_succeeded){
        free_thread_action(trainer_thread_context -> thread_action);
        trainer_thread_context -> thread_action = catch_thread_action_for(localizable_pokemon);
        log_succesfully_caught(localizable_pokemon);
        catch_action_completed_successfully_by(trainer_thread_context);
    }else{
        log_failed_caught_of(localizable_pokemon);
        remove_occurrence_of(localizable_pokemon);
        catch_action_failed_by(trainer_thread_context);
    }
}

t_thread_action* waiting_catch_response_thread_action_for(t_localizable_object* localizable_pokemon, int message_id){

    t_waiting_catch_response_action* waiting_catch_response_action =
            safe_malloc(sizeof(t_waiting_catch_response_action));

    waiting_catch_response_action -> localizable_pokemon = localizable_pokemon;
    waiting_catch_response_action -> message_id = message_id;
    waiting_catch_response_action -> caught_succeeded = false;

    t_thread_action* thread_action = new_thread_action();
    thread_action -> request -> operation = WAITING_CATCH_RESPONSE;
    thread_action -> request -> structure = waiting_catch_response_action;
    thread_action -> request -> sanitizer_function = free;
    thread_action -> execution_function = (void (*)(void *)) waiting_catch_response_action_execution_function;

    return thread_action;
}

t_thread_action* waiting_for_more_pokemons_thread_action(){

    t_thread_action* thread_action = new_thread_action();
    thread_action -> request -> operation = WAITING_FOR_MORE_POKEMONS;
    thread_action -> request -> structure = NULL;
    thread_action -> request -> sanitizer_function = free;
    thread_action -> execution_function = NULL;

    return thread_action;
}

void free_waiting_for_exchange_action(t_waiting_for_exchange_action* waiting_for_exchange_action){
    list_destroy(waiting_for_exchange_action -> pokemons_in_excess);
    list_destroy(waiting_for_exchange_action -> required_pokemons_not_caught);
    free(waiting_for_exchange_action);
}

t_thread_action* waiting_for_exchange_thread_action_for(t_trainer* trainer){

    t_list* required_pokemons_not_caught = requirements_of(trainer);
    t_list* pokemons_in_excess = pokemons_in_excess_of(trainer);

    t_waiting_for_exchange_action* waiting_for_exchange_action =
            safe_malloc(sizeof(t_waiting_for_exchange_action));

    waiting_for_exchange_action -> pokemons_in_excess = pokemons_in_excess;
    waiting_for_exchange_action -> required_pokemons_not_caught = required_pokemons_not_caught;

    t_thread_action* thread_action = new_thread_action();
    thread_action -> request -> operation = WAITING_FOR_EXCHANGE;
    thread_action -> request -> structure = waiting_for_exchange_action;
    thread_action -> request -> sanitizer_function = (void (*)(void *)) free_waiting_for_exchange_action;
    thread_action -> execution_function = NULL;

    return thread_action;
}