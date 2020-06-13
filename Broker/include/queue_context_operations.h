#ifndef DELIBIRD_QUEUE_CONTEXT_OPERATIONS_H
#define DELIBIRD_QUEUE_CONTEXT_OPERATIONS_H

typedef struct Queue_context_operations{
    void (*add_subscriber_function) (void*, void*);
    void (*push_message_status_to_queue_function) (void*, void*);
    void (*remove_message_status_of_queue_function) (void*, void*);
}t_queue_context_operations;

t_queue_context_operations* new_queue_context_operations();

#endif //DELIBIRD_QUEUE_CONTEXT_OPERATIONS_H
