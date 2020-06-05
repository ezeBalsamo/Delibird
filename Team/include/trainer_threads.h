#ifndef DELIBIRD_TRAINER_THREADS_H
#define DELIBIRD_TRAINER_THREADS_H

#include <commons/collections/list.h>
#include "team_manager.h"

enum Thread_states{
    NEW, READY, EXECUTE, BLOCKED, FINISHED
};

typedef struct Trainer_thread_context{
    t_localizable_object* localizable_trainer;
    sem_t semaphore;
    uint32_t state;
    void* thread_action;
    void (*execution_function) (void*);
}t_trainer_thread_context;

void initialize_trainer_threads();

void execute_trainer_thread_context_action(t_trainer_thread_context* trainer_thread_context);

#endif //DELIBIRD_TRAINER_THREADS_H
