#ifndef DELIBIRD_CHAINED_EVALUATION_H
#define DELIBIRD_CHAINED_EVALUATION_H

#include <stdbool.h>
#include <stdint.h>

typedef struct Basic_evaluation{
    bool (*satisfy_function) (void*);
    void (*success_function) (void*);
    void (*failure_function) (void*);
}t_basic_evaluation;

typedef struct Chained_on_failure_evaluation{
    bool (*satisfy_function) (void*);
    void (*success_function) (void*);
    void* next_on_failure_evaluation;
}t_chained_on_failure_evaluation;

typedef struct Chained_on_succesful_evaluation{
    bool (*satisfy_function) (void*);
    void* next_on_succesful_evaluation;
    void (*failure_function) (void*);
}t_chained_on_succesful_evaluation;

#endif //DELIBIRD_CHAINED_EVALUATION_H
