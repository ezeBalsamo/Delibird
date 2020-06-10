#ifndef DELIBIRD_CHAINED_EVALUATION_H
#define DELIBIRD_CHAINED_EVALUATION_H

#include <stdbool.h>
#include <stdint.h>

enum Chained_evaluation_types{
    CHAINED_ON_SUCCESFUL, CHAINED_ON_FAILURE, BASIC
};

typedef struct Chained_evaluation_type{
    uint32_t chained_evaluation_type;
    void* evaluation;
}t_identified_chained_evaluation;

typedef struct Basic_evaluation{
    bool (*satisfy_function) (void*);
    void (*success_function) (void*);
    void (*failure_function) (void*);
}t_basic_evaluation;

typedef struct Chained_on_failure_evaluation{
    bool (*satisfy_function) (void*);
    void (*success_function) (void*);
    t_identified_chained_evaluation* next_evaluation;
}t_chained_on_failure_evaluation;

typedef struct Chained_on_succesful_evaluation{
    bool (*satisfy_function) (void*);
    t_identified_chained_evaluation* next_evaluation;
    void (*failure_function) (void*);
}t_chained_on_succesful_evaluation;

void execute_evaluation_for(t_identified_chained_evaluation* identified_chained_evaluation, void* object_to_evaluate);

void free_chained_evaluation(t_identified_chained_evaluation* identified_chained_evaluation);

#endif //DELIBIRD_CHAINED_EVALUATION_H
