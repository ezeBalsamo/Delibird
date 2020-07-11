#ifndef DELIBIRD_DEADLOCK_DETECTER_H
#define DELIBIRD_DEADLOCK_DETECTER_H

#include <stdbool.h>

bool deadlock_exists();
void calculate_and_notify_deadlocks_quantity();

#endif //DELIBIRD_DEADLOCK_DETECTER_H
