#ifndef DELIBIRD_TRAINER_THREADS_H
#define DELIBIRD_TRAINER_THREADS_H

#include <commons/collections/list.h>
#include "team_manager.h"
#include "../../Utils/include/common_structures.h"

enum Thread_states{
    NEW, READY, EXECUTE, BLOCKED, FINISHED
};

enum thread_action_types{
    MOVEMENT,
    CATCH,
    WAITING_CATCH_RESPONSE,
    WAITING_FOR_MORE_POKEMONS,
    WAITING_FOR_EXCHANGE,
    EXCHANGE,
    NULL_THREAD_ACTION
};

typedef struct Thread_action{
    t_request* request;
    void (*execution_function) (void*);
}t_thread_action;

typedef struct Trainer_thread_context{
    t_localizable_object* localizable_trainer;
    sem_t semaphore;
    uint32_t state;
    t_thread_action* thread_action;
}t_trainer_thread_context;

void initialize_trainer_threads();

bool are_equal_trainer_thread_contexts(t_trainer_thread_context* trainer_thread_context, t_trainer_thread_context* another_trainer_thread_context);

void execute_trainer_thread_context_action(t_trainer_thread_context* trainer_thread_context);
t_thread_action* new_thread_action();
t_thread_action* new_null_thread_action();
void* internal_thread_action_in(t_trainer_thread_context* trainer_thread_context);
int internal_thread_action_type_in(t_trainer_thread_context* trainer_thread_context);
t_trainer* trainer_from_thread_context(t_trainer_thread_context* trainer_thread_context);

void free_thread_action(t_thread_action* thread_action);
void free_trainer_threads();

#endif //DELIBIRD_TRAINER_THREADS_H
