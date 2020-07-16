#include <commons/collections/list.h>
#include <dispatcher_queues.h>
#include "deadlock_detecter.h"

bool is_dispatcher_queue_empty(t_dispatcher_queue* dispatcher_queue){
    bool is_empty;

    void _is_empty(){
        is_empty = list_is_empty(dispatcher_queue -> trainer_thread_contexts);
    }

    handling_concurrency_do(dispatcher_queue, _is_empty);
    return is_empty;
}

bool is_waiting_for_trade(t_trainer_thread_context* trainer_thread_context){
    return internal_thread_action_type_in(trainer_thread_context) == WAITING_FOR_TRADE;
}

bool are_all_blocked_trainer_thread_contexts_waiting_for_trade(){

    t_dispatcher_queue* dispatcher_queue = dispatcher_queue_of(BLOCKED);
    bool are_all_waiting_for_trade;

    void _are_all_trainer_thread_contexts_waiting_for_trade(){
        are_all_waiting_for_trade =
                list_all_satisfy(dispatcher_queue -> trainer_thread_contexts,
                                 (bool (*)(void *)) is_waiting_for_trade);
    }

    handling_concurrency_do(dispatcher_queue, _are_all_trainer_thread_contexts_waiting_for_trade);
    return are_all_waiting_for_trade;
}

bool deadlock_exists(){

    t_list* queues_expected_to_be_empty = list_create();
    list_add(queues_expected_to_be_empty, dispatcher_queue_of(NEW));
    list_add(queues_expected_to_be_empty, dispatcher_queue_of(READY));

    bool deadlock_has_occurred =
            list_all_satisfy(queues_expected_to_be_empty, (bool (*)(void *)) is_dispatcher_queue_empty) &&
            are_all_blocked_trainer_thread_contexts_waiting_for_trade();

    list_destroy(queues_expected_to_be_empty);
    return deadlock_has_occurred;
}