#ifndef DELIBIRD_NON_PREEMPTIVE_SJF_SCHEDULING_ALGORITHM_H
#define DELIBIRD_NON_PREEMPTIVE_SJF_SCHEDULING_ALGORITHM_H

#include "scheduling_algorithm.h"

typedef struct Trainer_remaining_time_estimator{
    t_trainer* trainer;
    double previous_estimation;
    int previous_real_time_consumed;
    double current_estimation;
}t_trainer_remaining_time_estimator;

void initialize_non_preemptive_sjf_scheduling_algorithm();

t_scheduling_algorithm* non_preemptive_sjf_scheduling_algorithm();

#endif //DELIBIRD_NON_PREEMPTIVE_SJF_SCHEDULING_ALGORITHM_H
