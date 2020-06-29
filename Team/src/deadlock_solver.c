#include <dispatcher_queues.h>
#include <distance_calculator.h>
#include <waiting_actions.h>
#include <commons/string.h>
#include <exchanges_inferrer.h>
#include <identified_exchanges_provider.h>
#include <trainer_thread_context_execution_cycle.h>
#include <team_logs_manager.h>
#include "deadlock_solver.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/pthread_wrapper.h"
#include <stdlib.h>

t_queue* remaining_identified_exchanges;
sem_t exchange_resolution_semaphore;
t_list* current_exchanges_in_process;

void initialize_deadlock_solver(){
    remaining_identified_exchanges = queue_create();
    sem_initialize(&exchange_resolution_semaphore);
    initialize_exchanges_inferrer();
}

void execute_all_remaining_exchanges(){

    while (!queue_is_empty(remaining_identified_exchanges)){
        current_exchanges_in_process = queue_pop(remaining_identified_exchanges);

        t_identified_exchange* first_identified_exchange = list_first(current_exchanges_in_process);
        t_trainer_thread_context* trainer_thread_context = first_identified_exchange -> exchange -> trainer_thread_context;
        t_localizable_object* localizable_trainer = first_identified_exchange -> exchange -> another_trainer_thread_context -> localizable_trainer;

        prepare_for_movement_action(trainer_thread_context, localizable_trainer);
        sem_wait(&exchange_resolution_semaphore);
        list_destroy_and_destroy_elements(current_exchanges_in_process, (void (*)(void *)) free_identified_exchange);
    }
}

t_list* current_identified_exchanges_in_process(){
    return current_exchanges_in_process;
}

t_list* exchanges_between(t_trainer_thread_context* trainer_thread_context, t_trainer_thread_context* another_trainer_thread_context){

    t_waiting_for_exchange_action* waiting_for_exchange_action = internal_thread_action_in(trainer_thread_context);
    t_waiting_for_exchange_action* another_waiting_for_exchange_action = internal_thread_action_in(another_trainer_thread_context);

    t_list* exchanges_for_trainer =
            list_intersection(waiting_for_exchange_action -> required_pokemons_not_caught,
                                another_waiting_for_exchange_action -> pokemons_in_excess,
                                (bool (*)(void *, void *)) string_equals_ignore_case);

    t_list* exchanges_for_another_trainer =
            list_intersection(another_waiting_for_exchange_action -> required_pokemons_not_caught,
                                waiting_for_exchange_action -> pokemons_in_excess,
                                (bool (*)(void *, void *)) string_equals_ignore_case);

    t_list* exchanges_inferred =
            infer_exchanges(trainer_thread_context, exchanges_for_trainer, another_trainer_thread_context, exchanges_for_another_trainer);

    list_destroy(exchanges_for_trainer);
    list_destroy(exchanges_for_another_trainer);
    return exchanges_inferred;
}

void calculate_and_load_exchanges_between_trainer_thread_context_and_all(t_trainer_thread_context* trainer_thread_context_to_compare, t_list* closest_trainer_thread_contexts){

    void _calculate_and_load_exchanges_for(t_trainer_thread_context* trainer_thread_context){
        t_list* identified_exchanges = exchanges_between(trainer_thread_context_to_compare, trainer_thread_context);
        if(!list_is_empty(identified_exchanges)){
            queue_push(remaining_identified_exchanges, identified_exchanges);
        }else{
            list_destroy(identified_exchanges);
        }
    }

    list_iterate(closest_trainer_thread_contexts, (void (*)(void *)) _calculate_and_load_exchanges_for);
}

void remove_finished_trainer_thread_contexts_from(t_list* trainer_thread_contexts){

    bool _has_finished(t_trainer_thread_context* trainer_thread_context){
        return trainer_thread_context -> state == FINISHED;
    }

    list_remove_all_by_condition(trainer_thread_contexts, (bool (*)(void *)) _has_finished);
}

void increase_range_of_view(t_list* closest_trainer_thread_contexts, t_list* remaining_trainer_thread_contexts){

    if(!list_is_empty(remaining_trainer_thread_contexts)){
        t_trainer_thread_context* closest_trainer_thread_context =
                closest_trainer_thread_context_to_trainer_thread_contexts(remaining_trainer_thread_contexts, closest_trainer_thread_contexts);

        bool _are_equals(t_trainer_thread_context* trainer_thread_context){
            return are_equal_trainer_thread_contexts(closest_trainer_thread_context, trainer_thread_context);
        }

        list_remove_by_condition(remaining_trainer_thread_contexts, (bool (*)(void *)) _are_equals);
        list_add(closest_trainer_thread_contexts, closest_trainer_thread_context);
    }
}

void resolve_deadlock_with(t_list* closest_trainer_thread_contexts, t_list* remaining_trainer_thread_contexts){

    t_list* duplicated_closest_trainer_thread_contexts = list_duplicate(closest_trainer_thread_contexts);

    for(int i = 0; i < (list_size(closest_trainer_thread_contexts) - 1); i++){

        t_trainer_thread_context* trainer_thread_context = list_remove_first(duplicated_closest_trainer_thread_contexts);
        calculate_and_load_exchanges_between_trainer_thread_context_and_all(trainer_thread_context, duplicated_closest_trainer_thread_contexts);
        execute_all_remaining_exchanges();
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
}

void free_deadlock_solver(){
    queue_destroy(remaining_identified_exchanges);
    sem_destroy(&exchange_resolution_semaphore);
    free_exchanges_inferrer();
}