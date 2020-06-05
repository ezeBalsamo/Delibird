#include <trainer_threads.h>
#include <team_logs_manager.h>
#include <dispatcher.h>
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/common_structures.h"

t_list* trainer_thread_contexts;
t_list* trainers_tids;

void execute_trainer_thread_context_action(t_trainer_thread_context* trainer_thread_context){
    trainer_thread_context -> execution_function (trainer_thread_context);
}

void join_trainers_threads(){
    for (int i = 0; i < list_size(trainers_tids); i++) {
        pthread_t* trainer_tid = (pthread_t*) list_get(trainers_tids, i);
        thread_join(*trainer_tid);
    }
}

void* trainer_thread(void* trainer_thread_context){
    t_trainer_thread_context* cast_trainer_thread_context = (t_trainer_thread_context*) trainer_thread_context;

    t_trainer* trainer = cast_trainer_thread_context -> localizable_trainer -> object;
    log_succesful_creation_of_thread_of_trainer(trainer-> sequential_number);

    while(cast_trainer_thread_context -> state != FINISHED){
        sem_wait(&cast_trainer_thread_context -> semaphore);
        execute_trainer_thread_context_action(cast_trainer_thread_context);
    }

    return NULL;
}

void initialize_and_load_trainer_thread_for(void* trainer_thread_context){

    pthread_t* trainer_tid = safe_malloc(sizeof(pthread_t));
    *trainer_tid = thread_create(trainer_thread, trainer_thread_context, log_trainer_thread_create_error);

    list_add(trainers_tids, (void*) trainer_tid);
    new_thread_created_for((t_trainer_thread_context*) trainer_thread_context);
}

void initialize_and_load_trainer_thread_context_for(t_localizable_object* localizable_trainer){
    sem_t trainer_semaphore;
    sem_init(&trainer_semaphore, false, 0);

    t_trainer_thread_context* trainer_thread_context = safe_malloc(sizeof(t_trainer_thread_context));
    trainer_thread_context -> localizable_trainer = localizable_trainer;
    trainer_thread_context -> semaphore = trainer_semaphore;

    list_add(trainer_thread_contexts, (void*) trainer_thread_context);
}

void initialize_trainer_thread_contexts(){
    trainer_thread_contexts = list_create();
    with_trainers_do(initialize_and_load_trainer_thread_context_for);
}

void initialize_trainer_threads(){

    initialize_trainer_thread_contexts();

    trainers_tids = list_create();
    list_iterate(trainer_thread_contexts, initialize_and_load_trainer_thread_for);

    join_trainers_threads();
}