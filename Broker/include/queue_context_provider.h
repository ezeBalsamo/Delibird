#ifndef TP_2020_1C_EL_KUELGUE_QUEUE_MESSAGE_MANAGER_H
#define TP_2020_1C_EL_KUELGUE_QUEUE_MESSAGE_MANAGER_H

#include <stdint.h>
#include <commons/collections/queue.h>
#include <zconf.h>
#include <commons/collections/dictionary.h>
#include <semaphore.h>
#include "../../Utils/include/common_structures.h"
#include "publisher_message_mode.h"
#include "queue_message_status.h"

typedef struct Queue_context{
    uint32_t operation;
    t_queue* queue;
    pthread_mutex_t queue_mutex;
    t_list* subscribers;
    pthread_mutex_t subscribers_mutex;
} t_queue_context;

t_queue_context* get_queue_context_of_queue_named(char* queue_name);

void initialize_queue_context_provider();

t_request* create_request_from(t_message_status* message_status);

void free_queue_context_provider();

#endif //TP_2020_1C_EL_KUELGUE_QUEUE_MESSAGE_MANAGER_H
