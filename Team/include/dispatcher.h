#ifndef DELIBIRD_DISPATCHER_H
#define DELIBIRD_DISPATCHER_H

#include <commons/collections/list.h>
#include "trainer_threads.h"

void initialize_dispatcher();

void new_thread_created_for(t_trainer_thread_context* t_trainer_thread_context);
t_list* schedulable_trainer_thread_contexts();
void trainer_thread_context_ready_to_be_sheduled(t_trainer_thread_context* trainer_thread_context);
bool is_anybody_executing();
void execute_trainer_thread_context();
void preempt_due_to(char* reason);
void trainer_thread_context_has_finished(t_trainer_thread_context* trainer_thread_context);
void assert_all_trainer_thread_contexts_have_finished();
int finished_trainer_thread_contexts_amount();

void free_dispatcher();

#endif //DELIBIRD_DISPATCHER_H
