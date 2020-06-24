#include "../include/dispatcher_queues.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/t_list_extension.h"
#include <stdlib.h>
#include <team_logs_manager.h>

t_list* dispatcher_queues;

t_dispatcher_queue* new_dispatcher_queue_for(uint32_t state){

    pthread_mutex_t* mutex = safe_malloc(sizeof(pthread_mutex_t));
    safe_mutex_initialize(mutex);

    t_dispatcher_queue* dispatcher_queue = safe_malloc(sizeof(t_dispatcher_queue));
    dispatcher_queue -> state = state;
    dispatcher_queue -> mutex = mutex;
    dispatcher_queue -> trainer_thread_contexts = list_create();

    return dispatcher_queue;
}

void initialize_dispatcher_queues(){
    dispatcher_queues = list_create();

    list_add(dispatcher_queues, new_dispatcher_queue_for(NEW));
    list_add(dispatcher_queues, new_dispatcher_queue_for(READY));
    list_add(dispatcher_queues, new_dispatcher_queue_for(EXECUTE));
    list_add(dispatcher_queues, new_dispatcher_queue_for(BLOCKED));
    list_add(dispatcher_queues, new_dispatcher_queue_for(FINISHED));
}

t_dispatcher_queue* dispatcher_queue_of(uint32_t state){

    bool _is_for(t_dispatcher_queue* dispatcher_queue){
        return dispatcher_queue -> state == state;
    }

    t_dispatcher_queue* dispatcher_queue_found =
            list_find(dispatcher_queues, (bool (*)(void *)) _is_for);

    if(!dispatcher_queue_found){
        log_invalid_state_error();
    }

    return dispatcher_queue_found;
}

t_list* trainer_thread_contexts_in(uint32_t state){
    return dispatcher_queue_of(state) -> trainer_thread_contexts;
}

void remove_from(t_list* list_to_search, t_trainer_thread_context* trainer_thread_context_to_find){

    bool _are_equals(t_trainer_thread_context* trainer_thread_context_to_compare){
        return are_equal_trainers(trainer_thread_context_to_find -> localizable_trainer -> object,
                                  trainer_thread_context_to_compare -> localizable_trainer -> object);
    }

    t_trainer_thread_context* trainer_thread_context_found =
            list_remove_by_condition(list_to_search, (bool (*)(void *)) _are_equals);

    if (!trainer_thread_context_found) {
        t_trainer* trainer = trainer_thread_context_to_find -> localizable_trainer -> object;
        log_trainer_thread_context_not_found_error_for(trainer -> sequential_number);
        free_system();
    }
}

void remove_from_execute(){

    t_dispatcher_queue* dispatcher_queue = dispatcher_queue_of(EXECUTE);
    t_trainer_thread_context* trainer_thread_context = list_get(dispatcher_queue -> trainer_thread_contexts, 0);
    pthread_mutex_lock(dispatcher_queue -> mutex);
    remove_from(dispatcher_queue -> trainer_thread_contexts, trainer_thread_context);
    pthread_mutex_unlock(dispatcher_queue -> mutex);
}

void remove_from_dispatcher_queue(t_trainer_thread_context* trainer_thread_context){

    t_dispatcher_queue* dispatcher_queue = dispatcher_queue_of(trainer_thread_context -> state);

    pthread_mutex_lock(dispatcher_queue -> mutex);
    remove_from(dispatcher_queue -> trainer_thread_contexts, trainer_thread_context);
    pthread_mutex_unlock(dispatcher_queue -> mutex);
}

void add_to_dispatcher_queue(t_trainer_thread_context* trainer_thread_context, uint32_t to_state){

    t_dispatcher_queue* dispatcher_queue = dispatcher_queue_of(to_state);

    pthread_mutex_lock(dispatcher_queue -> mutex);
    list_add(dispatcher_queue -> trainer_thread_contexts, trainer_thread_context);
    pthread_mutex_unlock(dispatcher_queue -> mutex);

    trainer_thread_context -> state = to_state;
}

void move_to(t_trainer_thread_context* trainer_thread_context, uint32_t to_state){

    remove_from_dispatcher_queue(trainer_thread_context);
    add_to_dispatcher_queue(trainer_thread_context, to_state);
}

void free_dispatcher_queue(t_dispatcher_queue* dispatcher_queue){
    pthread_mutex_destroy(dispatcher_queue -> mutex);
    free(dispatcher_queue -> mutex);
    list_destroy(dispatcher_queue -> trainer_thread_contexts);
    free(dispatcher_queue);
}

void free_dispatcher_queues(){
    list_destroy_and_destroy_elements(dispatcher_queues, (void (*)(void *)) free_dispatcher_queue);
}