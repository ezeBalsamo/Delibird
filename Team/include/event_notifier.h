#ifndef DELIBIRD_EVENT_NOTIFIER_H
#define DELIBIRD_EVENT_NOTIFIER_H

enum Event_types{
    CURRENT_EXECUTION_STOPPED,
    EXECUTION_CYCLE_CONSUMED,
    CONTEXT_SWITCH_REALIZED,
    EXECUTION_CYCLE_CONSUMED_BY_TRAINER,
    PRODUCED_AND_SOLVED_DEADLOCK
};

typedef struct subscription{
    int event_type;
    void (*callback_function) (void* event_argument);
}t_subscription;

void initialize_event_notifier();
void subscribe_to_event_doing(int event_type, void (*callback_function) (void* event_argument));
void notify(int event_type);
void notify_with_argument(int event_type, void* event_argument);
void free_event_notifier();

#endif //DELIBIRD_EVENT_NOTIFIER_H
