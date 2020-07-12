#ifndef DELIBIRD_BASIC_SJF_SCHEDULING_ALGORITHM_BEHAVIOUR_H
#define DELIBIRD_BASIC_SJF_SCHEDULING_ALGORITHM_BEHAVIOUR_H

#include "team_manager.h"
#include "trainer_threads.h"

typedef struct Trainer_remaining_time_estimator{
    t_trainer* trainer;
    double previous_estimation;
    int previous_real_time_consumed;
    double current_estimation;
}t_trainer_remaining_time_estimator;

void initialize_basic_sjf_scheduling_algorithm_behaviour();

double current_estimation_of_trainer_thread_context(t_trainer_thread_context* trainer_thread_context);
double remaining_estimation_of(t_trainer* trainer);
t_trainer_remaining_time_estimator* trainer_remaining_time_estimator_of(t_trainer* trainer);
void basic_sjf_update_ready_queue_when_adding(t_list* ready_trainer_thread_contexts, t_trainer_thread_context* trainer_thread_context);

#endif //DELIBIRD_BASIC_SJF_SCHEDULING_ALGORITHM_BEHAVIOUR_H
