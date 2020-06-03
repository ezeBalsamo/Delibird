#include <trainer_threads.h>
#include <team_logs_manager.h>
#include <dispatcher.h>
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/common_structures.h"

t_list* trainers_semaphores;
t_list* trainers_tids;

void join_trainers_threads(){
    for (int i = 0; i < list_size(trainers_tids); i++) {
        pthread_t* trainer_tid = (pthread_t*) list_get(trainers_tids, i);
        thread_join(*trainer_tid);
    }
}

void* trainer_thread(void* synchronizable_trainer){
    t_synchronizable_trainer* cast_synchronizable_trainer = (t_synchronizable_trainer*) synchronizable_trainer;

    log_succesful_creation_of_thread_of_trainer(cast_synchronizable_trainer -> trainer -> sequential_number);
    sem_wait(&cast_synchronizable_trainer -> semaphore);

    return NULL;
}

void initialize_and_load_trainer_thread_for(void* synchronizable_trainer){

    pthread_t* trainer_tid = safe_malloc(sizeof(pthread_t));
    *trainer_tid = thread_create(trainer_thread, synchronizable_trainer, log_trainer_thread_create_error);

    list_add(trainers_tids, (void*) trainer_tid);
    new_thread_created_for((t_synchronizable_trainer*) synchronizable_trainer);
}

void initialize_and_load_synchronizable_trainer_for(t_trainer* trainer){
    sem_t trainer_semaphore;
    sem_init(&trainer_semaphore, false, 0);

    t_synchronizable_trainer* synchronizable_trainer = safe_malloc(sizeof(t_synchronizable_trainer));
    synchronizable_trainer -> trainer = trainer;
    synchronizable_trainer -> semaphore = trainer_semaphore;

    list_add(trainers_semaphores, (void*) synchronizable_trainer);
}

void initialize_trainers_semaphores(){
    trainers_semaphores = list_create();
    with_trainers_do(initialize_and_load_synchronizable_trainer_for);
}

void initialize_trainer_threads(){

    initialize_trainers_semaphores();

    trainers_tids = list_create();
    list_iterate(trainers_semaphores, initialize_and_load_trainer_thread_for);

    join_trainers_threads();
}