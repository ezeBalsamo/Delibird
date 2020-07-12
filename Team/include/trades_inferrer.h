#ifndef DELIBIRD_TRADES_INFERRER_H
#define DELIBIRD_TRADES_INFERRER_H

#include <commons/collections/list.h>
#include "trainer_threads.h"

typedef struct Trade_inferrer{
    bool (*can_handle_function) (t_list*, t_list*);
    t_list* (*infer_function) (t_trainer_thread_context*, t_list*, t_trainer_thread_context*, t_list*);
}t_trade_inferrer;

void initialize_trades_inferrer();

t_list* infer_trades(t_trainer_thread_context* trainer_thread_context,
                     t_list* benefitial_trades_for_first,
                     t_trainer_thread_context* another_trainer_thread_context,
                     t_list* benefitial_trades_for_second);

void free_trades_inferrer();

#endif //DELIBIRD_TRADES_INFERRER_H
