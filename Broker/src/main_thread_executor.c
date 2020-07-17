#include <connection_handler.h>
#include "main_thread_executor.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "../../Utils/include/common_structures.h"

pthread_t multithreaded_server_tid;
sem_t connection_handler_semaphore;

void initialize_main_thread_executor(){
    safe_sem_initialize(&connection_handler_semaphore);
}

void execute_main_thread(){
    multithreaded_server_tid = default_safe_thread_create(initialize_connection_handler, NULL);
    safe_sem_wait(&connection_handler_semaphore);
}

void free_main_thread_executor(){
    safe_thread_cancel(multithreaded_server_tid);

    safe_sem_post(&connection_handler_semaphore);
    safe_sem_destroy(&connection_handler_semaphore);
}
