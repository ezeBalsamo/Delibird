#ifndef DELIBIRD_METRICS_H
#define DELIBIRD_METRICS_H

typedef struct Execution_cycles_quantity_per_trainer{
    t_trainer* trainer;
    int execution_cycles_quantity;
}t_execution_cycles_quantity_per_trainer;

void initialize_metrics();
void dump_metrics();
void free_metrics();

#endif //DELIBIRD_METRICS_H
