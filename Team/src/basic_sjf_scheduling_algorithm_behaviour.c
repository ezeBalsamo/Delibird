#include <stdlib.h>
#include <team_logs_manager.h>
#include <event_notifier.h>
#include "basic_sjf_scheduling_algorithm_behaviour.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/t_list_extension.h"

double alpha;
t_list* trainer_remaining_time_estimators;

bool is_trainer_related_to_estimator(t_trainer* trainer, t_trainer_remaining_time_estimator* trainer_remaining_time_estimator){
    return are_equal_trainers(trainer_remaining_time_estimator -> trainer, trainer);
}

t_trainer_remaining_time_estimator* trainer_remaining_time_estimator_of(t_trainer* trainer){

    bool _is_for(t_trainer_remaining_time_estimator* trainer_remaining_time_estimator){
        return is_trainer_related_to_estimator(trainer, trainer_remaining_time_estimator);
    }

    t_trainer_remaining_time_estimator* trainer_remaining_time_estimator_found =
            list_find(trainer_remaining_time_estimators, (bool (*)(void *)) _is_for);

    if(!trainer_remaining_time_estimator_found){
        log_trainer_remaining_time_estimator_not_found_error_for(trainer);
        free_system();
    }

    return trainer_remaining_time_estimator_found;
}

t_trainer_remaining_time_estimator* trainer_remaining_time_estimator_related_to_thread_context(t_trainer_thread_context* trainer_thread_context){
    t_trainer* trainer = trainer_from_thread_context(trainer_thread_context);
    return trainer_remaining_time_estimator_of(trainer);
}

void initialize_alpha(){
    alpha = config_get_double_at("ALPHA");
}

void increment_previous_real_time_consumed_by_trainer(t_trainer* trainer){
    t_trainer_remaining_time_estimator* trainer_remaining_time_estimator = trainer_remaining_time_estimator_of(trainer);
    trainer_remaining_time_estimator -> previous_real_time_consumed++;
}

void update_previous_estimation(t_trainer_thread_context* trainer_thread_context){
    t_trainer_remaining_time_estimator* trainer_remaining_time_estimator = trainer_remaining_time_estimator_related_to_thread_context(trainer_thread_context);
    trainer_remaining_time_estimator -> previous_estimation = trainer_remaining_time_estimator -> current_estimation;
}

static void subscribe_to_events(){

    subscribe_to_event_doing(EXECUTION_CYCLE_CONSUMED_BY_TRAINER,
                             (void (*)(void *)) increment_previous_real_time_consumed_by_trainer);

    subscribe_to_event_doing(CONTEXT_SWITCH_REALIZED, (void (*)(void *)) update_previous_estimation);
}

void free_trainer_remaining_time_estimators(){
    list_destroy_and_destroy_elements(trainer_remaining_time_estimators, free);
}

void initialize_basic_sjf_scheduling_algorithm_behaviour(){
    initialize_alpha();
    subscribe_to_events();

    trainer_remaining_time_estimators = list_create();
    consider_as_garbage(trainer_remaining_time_estimators, free_trainer_remaining_time_estimators);
}

void initialize_and_load_remaining_time_estimator_for(t_trainer* trainer){
    t_trainer_remaining_time_estimator* trainer_remaining_time_estimator = safe_malloc(sizeof(t_trainer_remaining_time_estimator));
    trainer_remaining_time_estimator -> trainer = trainer;
    trainer_remaining_time_estimator -> previous_estimation = config_get_double_at("ESTIMACION_INICIAL");
    trainer_remaining_time_estimator -> previous_real_time_consumed = 0;

    list_add(trainer_remaining_time_estimators, trainer_remaining_time_estimator);
}

void consider_as_new(t_trainer_thread_context* trainer_thread_context){
    t_trainer* trainer = trainer_from_thread_context(trainer_thread_context);

    bool already_exists =
            list_includes(trainer_remaining_time_estimators,
                          trainer,
                          (bool (*)(void *, void *)) is_trainer_related_to_estimator);

    if(!already_exists){
        initialize_and_load_remaining_time_estimator_for(trainer);
    }
}

void calculate_current_estimation_for(t_trainer_remaining_time_estimator* trainer_remaining_time_estimator){

    double previous_estimation = trainer_remaining_time_estimator -> previous_estimation;
    int previous_real_time_consumed = trainer_remaining_time_estimator -> previous_real_time_consumed;

    double current_estimation = previous_estimation * alpha + (double) (previous_real_time_consumed * (1 - alpha));
    trainer_remaining_time_estimator -> current_estimation = current_estimation;
}

void reset_previous_real_time_consumed_of(t_trainer_remaining_time_estimator* trainer_remaining_time_estimator){
    trainer_remaining_time_estimator -> previous_real_time_consumed = 0;
}

void insert_sorted(t_trainer_thread_context* trainer_thread_context,
                   t_trainer_remaining_time_estimator* trainer_remaining_time_estimator,
                   t_list* ready_trainer_thread_contexts){

    bool _is_shortest_than(t_trainer_thread_context* trainer_thread_context_to_compare){
        t_trainer_remaining_time_estimator* trainer_remaining_time_estimator_to_compare = trainer_remaining_time_estimator_related_to_thread_context(
                trainer_thread_context_to_compare);
        return trainer_remaining_time_estimator_to_compare -> current_estimation <= trainer_remaining_time_estimator -> current_estimation;
    }

    t_list* shortest_trainer_thread_contexts = list_filter(ready_trainer_thread_contexts, (bool (*)(void *)) _is_shortest_than);
    int sort_index = list_size(shortest_trainer_thread_contexts);
    list_add_in_index(ready_trainer_thread_contexts, sort_index, trainer_thread_context);

    list_destroy(shortest_trainer_thread_contexts);
    reset_previous_real_time_consumed_of(trainer_remaining_time_estimator);
}

void basic_sjf_update_ready_queue_when_adding(t_list* ready_trainer_thread_contexts, t_trainer_thread_context* trainer_thread_context){
    consider_as_new(trainer_thread_context);

    t_trainer_remaining_time_estimator* trainer_remaining_time_estimator =
            trainer_remaining_time_estimator_related_to_thread_context(trainer_thread_context);

    calculate_current_estimation_for(trainer_remaining_time_estimator);

    insert_sorted(trainer_thread_context, trainer_remaining_time_estimator, ready_trainer_thread_contexts);
}

double remaining_estimation_of(t_trainer* trainer){

    t_trainer_remaining_time_estimator* trainer_remaining_time_estimator = trainer_remaining_time_estimator_of(trainer);
    return trainer_remaining_time_estimator -> current_estimation - (double) trainer_remaining_time_estimator -> previous_real_time_consumed;
}

double current_estimation_of_trainer_thread_context(t_trainer_thread_context* trainer_thread_context){

    t_trainer_remaining_time_estimator* trainer_remaining_time_estimator = trainer_remaining_time_estimator_related_to_thread_context(trainer_thread_context);
    return trainer_remaining_time_estimator -> current_estimation;
}