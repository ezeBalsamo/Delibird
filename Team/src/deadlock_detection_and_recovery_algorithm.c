#include <deadlock_detecter.h>
#include <deadlock_solver.h>
#include "deadlock_detection_and_recovery_algorithm.h"

void detect_and_recover_from_deadlock(){
    if(deadlock_has_occurred()){
        recover_from_deadlock();
    }
}