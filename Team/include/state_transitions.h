#ifndef DELIBIRD_STATE_TRANSITIONS_H
#define DELIBIRD_STATE_TRANSITIONS_H

#include <stdint.h>
#include "trainer_threads.h"

typedef struct State_transition{
    uint32_t from_state;
    uint32_t to_state;
    void (*state_transition_function) (t_trainer_thread_context* trainer_thread_context);
}t_state_transition;

void initialize_state_transitions();

t_state_transition* state_transition_for(t_trainer_thread_context* trainer_thread_context, uint32_t to_state);

void free_state_transitions();

#endif //DELIBIRD_STATE_TRANSITIONS_H
