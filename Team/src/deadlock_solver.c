#include <dispatcher_queues.h>
#include <distance_calculator.h>
#include <waiting_actions.h>
#include <commons/string.h>
#include <exchanges_inferrer.h>
#include "deadlock_solver.h"
#include "../../Utils/include/t_list_extension.h"

t_list* remaining_exchanges;

void execute_all_remaining_exchanges(){
    while(!list_is_empty(remaining_exchanges)){
        //TODO: lógica de ejecución
    }
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

    return infer_exchanges(trainer_thread_context, exchanges_for_trainer, another_trainer_thread_context, exchanges_for_another_trainer);
}

void calculate_and_load_exchanges_between_trainer_thread_context_and_all(t_trainer_thread_context* trainer_thread_context_to_compare, t_list* closest_trainer_thread_contexts){

    void _calculate_and_load_exchanges_for(t_trainer_thread_context* trainer_thread_context){
        t_list* exchanges = exchanges_between(trainer_thread_context_to_compare, trainer_thread_context);
        list_add_all(remaining_exchanges, exchanges);
        list_destroy(exchanges);
    }

    list_iterate(closest_trainer_thread_contexts, (void (*)(void *)) _calculate_and_load_exchanges_for);
}

void increase_range_of_view(t_list* closest_trainer_thread_contexts, t_list* remaining_trainer_thread_contexts){
    //TODO: lógica de mover entrenadores de remaining a closest cuando no haya intercambios posibles en remaining
}

void resolve_deadlock_with(t_list* closest_trainer_thread_contexts, t_list* remaining_trainer_thread_contexts){

    t_list* duplicated_closest_trainer_thread_contexts = list_duplicate(closest_trainer_thread_contexts);

    for(int i = 0; i < list_size(closest_trainer_thread_contexts) - 1; i++){

        t_trainer_thread_context* trainer_thread_context = list_remove_first(duplicated_closest_trainer_thread_contexts);
        calculate_and_load_exchanges_between_trainer_thread_context_and_all(trainer_thread_context, duplicated_closest_trainer_thread_contexts);
        execute_all_remaining_exchanges();
    }

    increase_range_of_view(closest_trainer_thread_contexts, remaining_trainer_thread_contexts);

    while(!list_is_empty(closest_trainer_thread_contexts) && !list_is_empty(remaining_trainer_thread_contexts)){
        list_destroy(duplicated_closest_trainer_thread_contexts);
        resolve_deadlock_with(closest_trainer_thread_contexts, remaining_trainer_thread_contexts);
    }
}

void initialize_deadlock_solver(){
    remaining_exchanges = list_create();
    initialize_exchanges_inferrer();
}

void free_deadlock_solver(){
    list_destroy(remaining_exchanges);
    free_exchanges_inferrer();
}

void recover_from_deadlock(){

    initialize_deadlock_solver();

    t_list* trainer_thread_contexts = trainer_thread_contexts_in(BLOCKED);

    t_list* closest_pair_of_trainer_thread_contexts =
            closest_pair_of_trainer_thread_contexts_in(trainer_thread_contexts);

    t_list* remaining_trainer_thread_contexts =
            list_difference(trainer_thread_contexts,
                            closest_pair_of_trainer_thread_contexts,
                            (bool (*)(void *, void *)) are_equal_trainer_thread_contexts);

    resolve_deadlock_with(closest_pair_of_trainer_thread_contexts, remaining_trainer_thread_contexts);

    free_deadlock_solver();
}