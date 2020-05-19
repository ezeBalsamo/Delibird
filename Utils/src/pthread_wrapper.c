#include "../include/pthread_wrapper.h"
#include <commons/process.h>
#include <stdio.h>
#include <stdlib.h>

void default_thread_create_error_response(){
    unsigned int process_id = process_getpid();
    printf("An error occurred while creating a new thread for process: %u\n", process_id);
    exit(EXIT_FAILURE);
}

pthread_t thread_create(void* (*thread_function) (void*), void* thread_argument, void (*error_response_strategy)()){
    pthread_t tid;
    if(pthread_create(&tid, NULL, thread_function, thread_argument) != 0){
        error_response_strategy();
    }
    return tid;
}

void thread_join(pthread_t thread){
    pthread_join(thread, NULL);
}

pthread_t default_safe_thread_create(void* (*thread_function) (void*), void* thread_argument){
    return thread_create(thread_function, thread_argument, default_thread_create_error_response);
}