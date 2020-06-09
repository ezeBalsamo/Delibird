#ifndef DELIBIRD_PTHREAD_WRAPPER_H
#define DELIBIRD_PTHREAD_WRAPPER_H

#include <pthread.h>

pthread_t thread_create(void* (*thread_function) (void*), void* thread_argument, void (*error_response_strategy)());
void thread_join(pthread_t thread);
pthread_t default_safe_thread_create(void* (*thread_function) (void*), void* thread_argument);
void safe_mutex_initialize(pthread_mutex_t* mutex);
void safe_thread_cancel(pthread_t tid);

#endif //DELIBIRD_PTHREAD_WRAPPER_H
