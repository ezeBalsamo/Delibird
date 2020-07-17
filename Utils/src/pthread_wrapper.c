#include "../include/pthread_wrapper.h"
#include <commons/process.h>
#include <general_logs.h>
#include <garbage_collector.h>
#include <commons/string.h>
#include <stdlib.h>
#include <asm/errno.h>

void default_thread_create_error_response(){
    unsigned int process_id = process_getpid();
    log_pthread_create_error(process_id);
    free_system();
}

pthread_t thread_create(void* (*thread_function) (void*), void* thread_argument, void (*error_response_strategy)()){
    pthread_t tid;
    if(pthread_create(&tid, NULL, thread_function, thread_argument) != 0){
        error_response_strategy();
    }
    return tid;
}

void safe_thread_join_waiting_value(pthread_t thread, void** return_value){

    if(pthread_join(thread, return_value) != 0){
        log_syscall_error("Error al ejecutar pthread_join");
        free_system();
    }
}

void safe_thread_join(pthread_t thread){
    safe_thread_join_waiting_value(thread, NULL);
}

pthread_t default_safe_thread_create(void* (*thread_function) (void*), void* thread_argument){
    return thread_create(thread_function, thread_argument, default_thread_create_error_response);
}

void safe_mutex_initialize(pthread_mutex_t* mutex){

    if(pthread_mutex_init(mutex, NULL) != 0){
        log_syscall_error("Error al inicializar un mutex");
        free_system();
    }
}

void safe_mutex_lock(pthread_mutex_t* mutex){

    if(pthread_mutex_lock(mutex) != 0){
        log_syscall_error("Error al intentar tomar el lock del mutex");
        free_system();
    }
}

void safe_mutex_unlock(pthread_mutex_t* mutex){

    if(pthread_mutex_unlock(mutex) != 0){
        log_syscall_error("Error al intentar liberar el lock del mutex");
        free_system();
    }
}

void safe_mutex_destroy(pthread_mutex_t* mutex){

    int destroy_result = pthread_mutex_destroy(mutex);

    if(destroy_result == EBUSY){
        safe_mutex_lock(mutex);
        safe_mutex_unlock(mutex);
        safe_mutex_destroy(mutex);
    }
}

void safe_thread_cancel(pthread_t thread){

    char* message = string_from_format("Error al cancelar el hilo %u", process_get_thread_id());

    if(pthread_cancel(thread) != 0){
        log_syscall_error(message);
        free(message);
        free_system();
    }

    void* cancel_result;
    safe_thread_join_waiting_value(thread, &cancel_result);

    if(cancel_result != PTHREAD_CANCELED){
        log_syscall_error(message);
        free(message);
        free_system();
    }

    free(message);
}

void safe_thread_pointer_cancel(pthread_t* thread){
    safe_thread_cancel(*thread);
    free(thread);
}