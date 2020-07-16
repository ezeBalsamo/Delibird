#include <team_manager.h>
#include <team_broker_connection_handler.h>
#include <team_gameboy_connection_handler.h>
#include "main_threads_executor.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/common_structures.h"

pthread_t team_manager_thread;
pthread_t broker_connection_handler_thread;
pthread_t gameboy_connection_handler_thread;

sem_t trainer_thread_created;
sem_t localized_trainers_created;

void wait_until_trainer_threads_are_created(){

    sem_wait(&localized_trainers_created);
    int total_trainers_amount = trainers_amount();

    for(int i = 0; i < total_trainers_amount; i++){
        sem_wait(&trainer_thread_created);
    }
}

void initialize_main_threads_executor(){
    safe_sem_initialize(&localized_trainers_created);
    safe_sem_initialize(&trainer_thread_created);
}

void execute_main_threads(){

    team_manager_thread = default_safe_thread_create(initialize_team_manager, NULL);
    wait_until_trainer_threads_are_created();

    broker_connection_handler_thread = default_safe_thread_create(initialize_team_broker_connection_handler, NULL);
    gameboy_connection_handler_thread = default_safe_thread_create(initialize_team_gameboy_connection_handler, NULL);

    safe_thread_join(team_manager_thread);
}

void free_main_threads_executor(){
    safe_sem_destroy(&localized_trainers_created);
    safe_sem_destroy(&trainer_thread_created);

    safe_thread_cancel(broker_connection_handler_thread);
    safe_thread_cancel(gameboy_connection_handler_thread);
}