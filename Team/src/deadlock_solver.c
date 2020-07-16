#include <dispatcher_queues.h>
#include <distance_calculator.h>
#include <waiting_actions.h>
#include <commons/string.h>
#include <trades_inferrer.h>
#include <identified_trades_provider.h>
#include <trainer_thread_context_execution_cycle.h>
#include <team_logs_manager.h>
#include "deadlock_solver.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/pthread_wrapper.h"
#include <stdlib.h>
#include <trades_calculator.h>

sem_t trade_resolution_semaphore;

t_queue* remaining_identified_trades;
t_list* current_trades_in_process;

void initialize_deadlock_solver(){
    remaining_identified_trades = queue_create();
    safe_sem_initialize(&trade_resolution_semaphore);
    initialize_trades_inferrer();
}

void execute_all_remaining_trades(){

    while (!queue_is_empty(remaining_identified_trades)){
        current_trades_in_process = queue_pop(remaining_identified_trades);

        t_identified_trade* first_identified_trade = list_first(current_trades_in_process);
        t_trainer_thread_context* trainer_thread_context = first_identified_trade -> trade -> first_party_trainer_thread_context;
        t_localizable_object* localizable_trainer = first_identified_trade -> trade -> second_party_trainer_thread_context -> localizable_trainer;

        prepare_for_movement_action(trainer_thread_context, localizable_trainer);
        sem_wait(&trade_resolution_semaphore);
        list_destroy_and_destroy_elements(current_trades_in_process, (void (*)(void *)) free_identified_trade);
    }
}

t_list* current_identified_trades_in_process(){
    return current_trades_in_process;
}

bool has_finished(t_trainer_thread_context* trainer_thread_context){
    return trainer_thread_context -> state == FINISHED;
}

bool has_not_finished(t_trainer_thread_context* trainer_thread_context){
    return !has_finished(trainer_thread_context);
}

void calculate_and_execute_trades_between(t_trainer_thread_context* trainer_thread_context, t_trainer_thread_context* another_trainer_thread_context){
    t_list* identified_trades = trades_between(trainer_thread_context, another_trainer_thread_context);

    if(!list_is_empty(identified_trades)){
        queue_push(remaining_identified_trades, identified_trades);
        execute_all_remaining_trades();
    }else{
        list_destroy(identified_trades);
    }
}

void calculate_and_execute_trades_between_trainer_thread_context_and_all(t_trainer_thread_context* trainer_thread_context_to_compare, t_list* closest_trainer_thread_contexts){

    void _calculate_and_execute_trades_for(t_trainer_thread_context* trainer_thread_context){

        if(has_not_finished(trainer_thread_context_to_compare)){
            calculate_and_execute_trades_between(trainer_thread_context_to_compare, trainer_thread_context);
        }
    }

    t_list* not_finished_trainer_thread_contexts = list_filter(closest_trainer_thread_contexts, (bool (*)(void *)) has_not_finished);
    list_iterate(not_finished_trainer_thread_contexts, (void (*)(void *)) _calculate_and_execute_trades_for);
    list_destroy(not_finished_trainer_thread_contexts);
}

void remove_finished_trainer_thread_contexts_from(t_list* trainer_thread_contexts){

    list_remove_all_by_condition(trainer_thread_contexts, (bool (*)(void *)) has_finished);
}

void increase_range_of_view(t_list* closest_trainer_thread_contexts, t_list* remaining_trainer_thread_contexts){

    if(!list_is_empty(remaining_trainer_thread_contexts)){
        t_trainer_thread_context* closest_trainer_thread_context =
                closest_trainer_thread_context_to_trainer_thread_contexts(remaining_trainer_thread_contexts, closest_trainer_thread_contexts);

        bool _are_equals(t_trainer_thread_context* trainer_thread_context){
            return are_equal_trainer_thread_contexts(closest_trainer_thread_context, trainer_thread_context);
        }

        list_remove_by_condition(remaining_trainer_thread_contexts, (bool (*)(void *)) _are_equals);
        list_add_as_first(closest_trainer_thread_contexts, closest_trainer_thread_context);
    }
}

void resolve_deadlock_with(t_list* closest_trainer_thread_contexts, t_list* remaining_trainer_thread_contexts){

    t_list* duplicated_closest_trainer_thread_contexts = list_duplicate(closest_trainer_thread_contexts);

    for(int i = 0; i < (list_size(closest_trainer_thread_contexts) - 1); i++){

        t_trainer_thread_context* trainer_thread_context = list_remove_first(duplicated_closest_trainer_thread_contexts);
        calculate_and_execute_trades_between_trainer_thread_context_and_all(trainer_thread_context, duplicated_closest_trainer_thread_contexts);
    }

    remove_finished_trainer_thread_contexts_from(closest_trainer_thread_contexts);
    increase_range_of_view(closest_trainer_thread_contexts, remaining_trainer_thread_contexts);

    if(!list_is_empty(closest_trainer_thread_contexts) || !list_is_empty(remaining_trainer_thread_contexts)){
        resolve_deadlock_with(closest_trainer_thread_contexts, remaining_trainer_thread_contexts);
    }

    list_destroy(duplicated_closest_trainer_thread_contexts);
}

void recover_from_deadlock(){

    t_list* trainer_thread_contexts = trainer_thread_contexts_in(BLOCKED);
    log_deadlock_solver_algorithm_has_begun_for(trainer_thread_contexts);

    t_list* closest_pair_of_trainer_thread_contexts =
            closest_pair_of_trainer_thread_contexts_in(trainer_thread_contexts);

    t_list* remaining_trainer_thread_contexts =
            list_difference(trainer_thread_contexts,
                            closest_pair_of_trainer_thread_contexts,
                            (bool (*)(void *, void *)) are_equal_trainer_thread_contexts);

    resolve_deadlock_with(closest_pair_of_trainer_thread_contexts, remaining_trainer_thread_contexts);
    list_destroy(closest_pair_of_trainer_thread_contexts);
    list_destroy(remaining_trainer_thread_contexts);

    log_deadlock_solver_has_finished();
}

void free_deadlock_solver(){
    queue_destroy(remaining_identified_trades);
    safe_sem_destroy(&trade_resolution_semaphore);
    free_trades_inferrer();
}