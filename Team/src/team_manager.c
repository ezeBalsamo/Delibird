#include <trainers_parser.h>
#include "goal_calculator.h"
#include "../include/team_manager.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/pthread_wrapper.h"
#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <team_logs_manager.h>
#include <map.h>

t_list* trainers;
t_list* global_goal;
t_list* trainers_semaphores;
t_list* trainers_tids;

bool global_goal_contains(char* pokemon_name){
    t_list* actual_global_goal = team_global_goal_according_to(trainers);

    bool _is_equal_pokemon(void *pokemon_goal){
        return string_equals_ignore_case(((t_pokemon_goal*) pokemon_goal) -> pokemon_name, pokemon_name);
    }

    return list_any_satisfy(actual_global_goal,_is_equal_pokemon);
}

void join_trainers_threads(){
    for (int i = 0; i < list_size(trainers_tids); i++) {
        pthread_t* trainer_tid = (pthread_t*) list_get(trainers_tids, i);
        thread_join(*trainer_tid);
    }
}

void* trainer_thread(void* trainer_with_lock){
    t_trainer_with_lock* cast_trainer_with_lock = (t_trainer_with_lock*) trainer_with_lock;

    printf("Soy el entrenador %d a punto de ser bloqueado\n",
           cast_trainer_with_lock -> trainer -> sequential_number);

    sem_wait(&cast_trainer_with_lock -> semaphore);

    printf("Soy el entrenador %d y debería estar bloqueado\n",
           cast_trainer_with_lock -> trainer -> sequential_number);

    return NULL;
}

void initialize_and_load_trainer_thread_for(void* trainer_with_lock){

    pthread_t* trainer_tid = malloc(sizeof(pthread_t));
    *trainer_tid = thread_create(trainer_thread, trainer_with_lock, log_trainer_thread_create_error);

    list_add(trainers_tids, (void*) trainer_tid);
}

void initialize_trainers_threads(){

    trainers_tids = list_create();
    list_iterate(trainers_semaphores, initialize_and_load_trainer_thread_for);
}

void initialize_and_load_trainer_with_lock_for(t_trainer* trainer){
    sem_t trainer_semaphore;
    sem_init(&trainer_semaphore, false, 0);

    t_trainer_with_lock* trainer_with_lock = malloc(sizeof(t_trainer_with_lock));
    trainer_with_lock -> trainer = trainer;
    trainer_with_lock -> semaphore = trainer_semaphore;

    list_add(trainers_semaphores, (void*) trainer_with_lock);
}

void initialize_trainers_semaphores(){
    trainers_semaphores = list_create();
    with_trainers_do(initialize_and_load_trainer_with_lock_for);
}

void* initialize_team_manager(){
    trainers = parsed_trainers();
    global_goal = team_global_goal_according_to(trainers);
    initialize_map();

    initialize_trainers_semaphores();
    initialize_trainers_threads();
    join_trainers_threads();

    return NULL;
}

t_list* trainers_x_positions(){
    void* _x_position_of(void* trainer){
        return (void*) &(((t_trainer*) trainer) -> pos_x);
    }

    return list_map(trainers, _x_position_of);
}

t_list* trainers_y_positions(){
    void* _y_position_of(void* trainer){
        return (void*) &(((t_trainer*) trainer) -> pos_y);
    }

    return list_map(trainers, _y_position_of);
}

void with_trainers_do(void (*closure) (t_trainer*)){
    list_iterate(trainers, (void (*)(void *)) closure);
}

void with_global_goal_do(void (*closure) (t_pokemon_goal*)){
    list_iterate(global_goal, (void (*)(void *)) closure);
}

void free_trainer(t_trainer *trainer){
    list_destroy_and_destroy_elements(trainer -> desired_pokemons, free);
    list_destroy_and_destroy_elements(trainer -> current_pokemons, free);
    free(trainer);
}
void free_team_manager(){
    list_destroy_and_destroy_elements(global_goal, (void (*)(void *)) free);
    list_destroy_and_destroy_elements(trainers, (void (*)(void *)) free_trainer);
}