#ifndef DELIBIRD_TRAINER_THREADS_H
#define DELIBIRD_TRAINER_THREADS_H

#include <commons/collections/list.h>
#include "team_manager.h"

typedef struct Synchronizable_trainer{
    t_trainer* trainer;
    sem_t semaphore;
}t_synchronizable_trainer;

void initialize_trainer_threads();

#endif //DELIBIRD_TRAINER_THREADS_H
