#include <commons/collections/list.h>
#include <dispatcher_queues.h>
#include <dispatcher.h>
#include "deadlock_detecter.h"

bool is_waiting_for_trade(t_trainer_thread_context* trainer_thread_context){
    return internal_thread_action_type_in(trainer_thread_context) == WAITING_FOR_TRADE;
}

bool deadlock_exists(){

    int expected_blocked_trainer_thread_contexts_amount =
            trainers_amount() - finished_trainer_thread_contexts_amount();

    t_list* blocked_trainer_thread_contexts = trainer_thread_contexts_in(BLOCKED);

    bool deadlock_has_occurred =
            list_size(blocked_trainer_thread_contexts) == expected_blocked_trainer_thread_contexts_amount
            &&
            list_all_satisfy(blocked_trainer_thread_contexts, (bool (*)(void *)) is_waiting_for_trade);

    return deadlock_has_occurred;
}