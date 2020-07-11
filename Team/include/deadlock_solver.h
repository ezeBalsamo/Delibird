#ifndef DELIBIRD_DEADLOCK_SOLVER_H
#define DELIBIRD_DEADLOCK_SOLVER_H

#include <commons/collections/list.h>

void initialize_deadlock_solver();
t_list* current_identified_trades_in_process();
void recover_from_deadlock();
void free_deadlock_solver();

#endif //DELIBIRD_DEADLOCK_SOLVER_H
