#include <goal_calculator.h>
#include <dispatcher.h>
#include <pokemon_occurrences.h>
#include <distance_calculator.h>
#include <trainer_thread_context_execution_cycle.h>
#include <identified_exchanges_provider.h>
#include "trainer_thread_context_state_chained_evaluation.h"
#include "../../Utils/include/t_list_extension.h"

t_identified_chained_evaluation* caught_success_chained_evaluation;
t_identified_chained_evaluation* caught_failed_chained_evaluation;
t_identified_chained_evaluation* identified_exchanges_completed_chained_evaluation;

bool can_be_moved_to_ready_function(t_trainer_thread_context* trainer_thread_context){
    (void) trainer_thread_context;

    t_list* pokemons_waiting_for_be_caught = not_yet_targeted_pokemons();
    bool are_there_pokemons = !list_is_empty(pokemons_waiting_for_be_caught);
    list_destroy(pokemons_waiting_for_be_caught);

    return are_there_pokemons;
}

void prepare_for_movement_action_function(t_trainer_thread_context* trainer_thread_context){
    t_list* pokemons_waiting_for_be_caught = not_yet_targeted_pokemons();

    t_targetable_object* targetable_pokemon =
            targetable_pokemon_closest_to_localizable_trainer(pokemons_waiting_for_be_caught,
                                                              trainer_thread_context -> localizable_trainer);

    list_destroy(pokemons_waiting_for_be_caught);

    targetable_pokemon -> is_being_targeted = true;
    prepare_for_movement_action(trainer_thread_context, targetable_pokemon -> localizable_pokemon);
}

bool can_be_scheduled_function(t_trainer_thread_context* trainer_thread_context){
    t_trainer* trainer = trainer_thread_context -> localizable_trainer -> object;
    return can_catch_pokemons(trainer);
}

t_basic_evaluation* ready_or_schedulable_blocked_state_chained_evaluation(){
    t_basic_evaluation* basic_evaluation = safe_malloc(sizeof(t_basic_evaluation));
    basic_evaluation -> satisfy_function = (bool (*)(void *)) can_be_moved_to_ready_function;
    basic_evaluation -> success_function = (void (*)(void *)) prepare_for_movement_action_function;
    basic_evaluation -> failure_function = (void (*)(void *)) prepare_for_waiting_for_more_pokemons_action;

    return basic_evaluation;
}

bool has_finished_function(t_trainer_thread_context* trainer_thread_context){
    t_trainer* trainer = trainer_thread_context -> localizable_trainer -> object;
    return has_accomplished_own_goal(trainer);
}

t_chained_on_succesful_evaluation* next_state_chained_evaluation_when_has_not_finished(){

    t_identified_chained_evaluation* next_evaluation = safe_malloc(sizeof(t_identified_chained_evaluation));
    next_evaluation -> chained_evaluation_type = BASIC;
    next_evaluation -> evaluation = ready_or_schedulable_blocked_state_chained_evaluation();

    t_chained_on_succesful_evaluation* chained_on_succesful_evaluation = safe_malloc(sizeof(t_chained_on_succesful_evaluation));
    chained_on_succesful_evaluation -> satisfy_function = (bool (*)(void *)) can_be_scheduled_function;
    chained_on_succesful_evaluation -> next_evaluation = next_evaluation;
    chained_on_succesful_evaluation -> failure_function = (void (*)(void *)) prepare_for_waiting_for_deadlock_resolution;

    return chained_on_succesful_evaluation;
}

void initialize_caught_success_chained_evaluation(){
    t_identified_chained_evaluation* next_evaluation = safe_malloc(sizeof(t_identified_chained_evaluation));
    next_evaluation -> chained_evaluation_type = CHAINED_ON_SUCCESFUL;
    next_evaluation -> evaluation = next_state_chained_evaluation_when_has_not_finished();

    t_chained_on_failure_evaluation* chained_on_failure_evaluation = safe_malloc(sizeof(t_chained_on_failure_evaluation));
    chained_on_failure_evaluation -> satisfy_function = (bool (*)(void *)) has_finished_function;
    chained_on_failure_evaluation -> success_function = (void (*)(void *)) trainer_thread_context_has_finished;
    chained_on_failure_evaluation -> next_evaluation = next_evaluation;

    caught_success_chained_evaluation = safe_malloc(sizeof(t_identified_chained_evaluation));
    caught_success_chained_evaluation -> chained_evaluation_type = CHAINED_ON_FAILURE;
    caught_success_chained_evaluation -> evaluation = chained_on_failure_evaluation;
}

void initialize_caught_failed_chained_evaluation(){
    t_basic_evaluation* basic_evaluation = ready_or_schedulable_blocked_state_chained_evaluation();

    caught_failed_chained_evaluation = safe_malloc(sizeof(t_identified_chained_evaluation));
    caught_failed_chained_evaluation -> chained_evaluation_type = BASIC;
    caught_failed_chained_evaluation -> evaluation = basic_evaluation;
}

bool is_benefitial_for_both_parties_function(t_list* identified_exchanges){

    bool _are_all_benefitial_for_both_parties(t_identified_exchange* identified_exchange){
        return identified_exchange -> exchange_type == BENEFITIAL_FOR_BOTH_PARTIES;
    }
    return list_all_satisfy(identified_exchanges, (bool (*)(void *)) _are_all_benefitial_for_both_parties);
}

void consider_graceful_finished_of(t_trainer_thread_context* trainer_thread_context){

    if(trainer_thread_context -> state == FINISHED){
        free_thread_action(trainer_thread_context -> thread_action);
        t_thread_action* null_thread_action = new_null_thread_action();
        trainer_thread_context -> thread_action = null_thread_action;
        sem_post(&trainer_thread_context -> semaphore);
    }
}

void benefitial_for_both_parties_exchanges_realized_function(t_list* identified_exchanges){

    t_identified_exchange* identified_exchange = list_first(identified_exchanges);
    t_trainer_thread_context* first_party_trainer_thread_context =
            identified_exchange -> exchange -> first_party_trainer_thread_context;
    t_trainer_thread_context* second_party_trainer_thread_context =
            identified_exchange -> exchange -> second_party_trainer_thread_context;

    trainer_thread_context_state_chained_evaluation_value_when_caught_success_for(first_party_trainer_thread_context);
    trainer_thread_context_state_chained_evaluation_value_when_caught_success_for(second_party_trainer_thread_context);
    consider_graceful_finished_of(second_party_trainer_thread_context);
}

bool is_benefitial_only_for_first_party_function(t_list* identified_exchanges){

    bool _is_benefitial_only_for_first_party(t_identified_exchange* identified_exchange){
        return identified_exchange -> exchange_type == BENEFITIAL_ONLY_FOR_FIRST_PARTY;
    }
    return list_any_satisfy(identified_exchanges, (bool (*)(void *)) _is_benefitial_only_for_first_party);
}

void benefitial_only_for_first_party_exchanges_realized_function(t_list* identified_exchanges){

    t_identified_exchange* identified_exchange = list_first(identified_exchanges);
    t_trainer_thread_context* first_party_trainer_thread_context =
            identified_exchange -> exchange -> first_party_trainer_thread_context;

    trainer_thread_context_state_chained_evaluation_value_when_caught_success_for(first_party_trainer_thread_context);
}

bool is_benefitial_only_for_second_party_function(t_list* identified_exchanges){

    bool _is_benefitial_only_for_second_party(t_identified_exchange* identified_exchange){
        return identified_exchange -> exchange_type == BENEFITIAL_ONLY_FOR_SECOND_PARTY;
    }
    return list_any_satisfy(identified_exchanges, (bool (*)(void *)) _is_benefitial_only_for_second_party);
}

void benefitial_only_for_second_party_exchanges_realized_function(t_list* identified_exchanges){

    // Dado a que el primero se muevo al lugar del segundo, no solo me interesa recalcular el
    // estado del segundo, sino que también me interesa que el primero pase de ejecución a bloqueado.
    benefitial_for_both_parties_exchanges_realized_function(identified_exchanges);
}

t_basic_evaluation* is_benefitial_only_for_second_party_chained_evaluation(){
    t_basic_evaluation* basic_evaluation = safe_malloc(sizeof(t_basic_evaluation));
    basic_evaluation -> satisfy_function = (bool (*)(void *)) is_benefitial_only_for_second_party_function;
    basic_evaluation -> success_function = (void (*)(void *)) benefitial_only_for_second_party_exchanges_realized_function;
    basic_evaluation -> failure_function = NULL;

    return basic_evaluation;
}

t_chained_on_failure_evaluation* next_state_chained_evaluation_when_not_benefitial_for_both_parties(){

    t_identified_chained_evaluation* next_evaluation = safe_malloc(sizeof(t_identified_chained_evaluation));
    next_evaluation -> chained_evaluation_type = BASIC;
    next_evaluation -> evaluation = is_benefitial_only_for_second_party_chained_evaluation();

    t_chained_on_failure_evaluation* chained_on_failure_evaluation = safe_malloc(sizeof(t_chained_on_failure_evaluation));
    chained_on_failure_evaluation -> satisfy_function = (bool (*)(void *)) is_benefitial_only_for_first_party_function;
    chained_on_failure_evaluation -> success_function = (void (*)(void *)) benefitial_only_for_first_party_exchanges_realized_function;
    chained_on_failure_evaluation -> next_evaluation = next_evaluation;
}

void initialize_identified_exchanges_completed_chained_evaluation(){
    t_identified_chained_evaluation* next_evaluation = safe_malloc(sizeof(t_identified_chained_evaluation));
    next_evaluation -> chained_evaluation_type = CHAINED_ON_FAILURE;
    next_evaluation -> evaluation = next_state_chained_evaluation_when_not_benefitial_for_both_parties();

    t_chained_on_failure_evaluation* chained_on_failure_evaluation = safe_malloc(sizeof(t_chained_on_failure_evaluation));
    chained_on_failure_evaluation -> satisfy_function = (bool (*)(void *)) is_benefitial_for_both_parties_function;
    chained_on_failure_evaluation -> success_function = (void (*)(void *)) benefitial_for_both_parties_exchanges_realized_function;
    chained_on_failure_evaluation -> next_evaluation = next_evaluation;

    identified_exchanges_completed_chained_evaluation = safe_malloc(sizeof(t_identified_chained_evaluation));
    identified_exchanges_completed_chained_evaluation -> chained_evaluation_type = CHAINED_ON_FAILURE;
    identified_exchanges_completed_chained_evaluation -> evaluation = chained_on_failure_evaluation;

}

void initialize_trainer_thread_context_state_chained_evaluation(){
    initialize_caught_success_chained_evaluation();
    initialize_caught_failed_chained_evaluation();
    initialize_identified_exchanges_completed_chained_evaluation();
}

void trainer_thread_context_state_chained_evaluation_value_when_caught_success_for(t_trainer_thread_context* trainer_thread_context){

    execute_evaluation_for(caught_success_chained_evaluation, trainer_thread_context);
}

void trainer_thread_context_state_chained_evaluation_value_when_caught_failed_for(t_trainer_thread_context* trainer_thread_context){

    execute_evaluation_for(caught_failed_chained_evaluation, trainer_thread_context);
}

void trainer_thread_context_state_chained_evaluation_value_when_exchanges_completed_for(t_list* identified_exchanges){

    execute_evaluation_for(identified_exchanges_completed_chained_evaluation, identified_exchanges);
}

void free_trainer_thread_context_state_chained_evaluation(){
    free_chained_evaluation(caught_success_chained_evaluation);
    free_chained_evaluation(caught_failed_chained_evaluation);
    free_chained_evaluation(identified_exchanges_completed_chained_evaluation);
}