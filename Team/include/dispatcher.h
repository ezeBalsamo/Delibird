#ifndef DELIBIRD_DISPATCHER_H
#define DELIBIRD_DISPATCHER_H

#include <commons/collections/list.h>
#include "trainer_threads.h"

void initialize_dispatcher();

t_list* schedulable_trainer_thread_contexts();
t_list* non_schedulable_blocked_trainer_thread_contexts();

void trainer_thread_context_ready_to_be_sheduled(t_trainer_thread_context* trainer_thread_context);
void schedule(t_trainer_thread_context* trainer_thread_context, char* reason);
void stop_current_execution_doing(void (*state_function) ());

void consider_continue_executing();
bool is_anybody_executing();
t_trainer_thread_context* trainer_thread_context_executing();
void execute_trainer_thread_context();

bool preemption_must_take_place();
void preempt();
void preemption_completed();

void register_trainer_thread_context_as_new(t_trainer_thread_context* trainer_thread_context);
void register_trainer_thread_context_as_blocked(t_trainer_thread_context* trainer_thread_context);
void consider_deadlock_already_happened_when_trainer_thread_context_finishes();
void trainer_thread_context_has_finished(t_trainer_thread_context* trainer_thread_context);
void trainer_thread_context_has_become_blocked(t_trainer_thread_context* trainer_thread_context);

void consider_deadlock_occurred_according_to(t_trainer_thread_context* trainer_thread_context);

bool is_deadlock_resolution_in_process();
void deadlock_solver_has_begun();
void deadlock_solver_has_ended();

void consider_ending();
int finished_trainer_thread_contexts_amount();

void free_dispatcher();

#endif //DELIBIRD_DISPATCHER_H
