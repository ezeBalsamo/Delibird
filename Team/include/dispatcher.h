#ifndef DELIBIRD_DISPATCHER_H
#define DELIBIRD_DISPATCHER_H

#include <commons/collections/list.h>
#include "trainer_threads.h"

void initialize_dispatcher();

void new_thread_created_for(t_synchronizable_trainer* synchronizable_trainer);

t_list* schedulable_threads();

void trainer_ready_to_be_sheduled(t_synchronizable_trainer* synchronizable_trainer);

#endif //DELIBIRD_DISPATCHER_H
