#ifndef DELIBIRD_DISPATCHING_REASONS_H
#define DELIBIRD_DISPATCHING_REASONS_H

#include "trainer_threads.h"

char* move_to_pokemon_reason_for(t_trainer_thread_context* trainer_thread_context);
char* catch_pokemon_reason_for(t_trainer_thread_context* trainer_thread_context);

char* thread_action_as_string(t_trainer_thread_context* trainer_thread_context);

#endif //DELIBIRD_DISPATCHING_REASONS_H
