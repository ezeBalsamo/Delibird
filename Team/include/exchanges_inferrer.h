#ifndef DELIBIRD_EXCHANGES_INFERRER_H
#define DELIBIRD_EXCHANGES_INFERRER_H

#include <commons/collections/list.h>
#include "trainer_threads.h"

typedef struct Exchange_inferrer{
    bool (*can_handle_function) (t_list*, t_list*);
    t_list* (*infer_function) (t_trainer_thread_context*, t_list*, t_trainer_thread_context*, t_list*);
}t_exchange_inferrer;

void initialize_exchanges_inferrer();

t_list* infer_exchanges(t_trainer_thread_context* trainer_thread_context, t_list* benefitial_exchanges_for_first,
                        t_trainer_thread_context* another_trainer_thread_context, t_list* benefitial_exchanges_for_second);

void free_exchanges_inferrer();

#endif //DELIBIRD_EXCHANGES_INFERRER_H
