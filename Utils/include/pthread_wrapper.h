#ifndef DELIBIRD_PTHREAD_WRAPPER_H
#define DELIBIRD_PTHREAD_WRAPPER_H

#include <pthread.h>

pthread_t thread_create(void* (*thread_function) (void*), void* thread_argument, void (*error_response_strategy)());
void thread_join(pthread_t thread);
void default_thread_create_error_response();
pthread_t default_safe_thread_create(void* (*thread_function) (void*), void* thread_argument);
#endif //DELIBIRD_PTHREAD_WRAPPER_H
