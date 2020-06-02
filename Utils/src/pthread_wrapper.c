#include "../include/pthread_wrapper.h"
#include <commons/process.h>
#include <stdio.h>
#include <general_logs.h>
#include <free_system.h>

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

void thread_join(pthread_t thread){
    if(pthread_join(thread, NULL) != 0){
        log_syscall_error("Error al ejecutar pthread_join");
        free_system();
    }
}

pthread_t default_safe_thread_create(void* (*thread_function) (void*), void* thread_argument){
    return thread_create(thread_function, thread_argument, default_thread_create_error_response);
}