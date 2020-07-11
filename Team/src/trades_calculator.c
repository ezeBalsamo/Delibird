#include <waiting_actions.h>
#include <commons/string.h>
#include <trades_inferrer.h>
#include "trades_calculator.h"
#include "../../Utils/include/t_list_extension.h"

t_list* trades_between(t_trainer_thread_context* trainer_thread_context, t_trainer_thread_context* another_trainer_thread_context){

    t_waiting_for_trade_action* waiting_for_trade_action = internal_thread_action_in(trainer_thread_context);
    t_waiting_for_trade_action* another_waiting_for_trade_action = internal_thread_action_in(another_trainer_thread_context);

    t_list* trades_for_trainer =
            list_intersection(waiting_for_trade_action -> required_pokemons_not_caught,
                              another_waiting_for_trade_action -> pokemons_in_excess,
                              (bool (*)(void *, void *)) string_equals_ignore_case);

    t_list* trades_for_another_trainer =
            list_intersection(another_waiting_for_trade_action -> required_pokemons_not_caught,
                              waiting_for_trade_action -> pokemons_in_excess,
                              (bool (*)(void *, void *)) string_equals_ignore_case);

    t_list* trades_inferred =
            infer_trades(trainer_thread_context, trades_for_trainer, another_trainer_thread_context,
                         trades_for_another_trainer);

    list_destroy(trades_for_trainer);
    list_destroy(trades_for_another_trainer);
    return trades_inferred;
}