#include <deadlock_detecter.h>
#include <deadlock_solver.h>
#include <team_logs_manager.h>
#include <dispatcher.h>
#include "deadlock_detection_and_recovery_algorithm.h"

void initialize_deadlock_detection_and_recovery_algorithm(){
    initialize_deadlock_solver();
}

void detect_and_recover_from_deadlock(){
    log_deadlock_detection_algorithm_has_begun();
    if(deadlock_has_occurred()){
        log_deadlock_detection_algorithm_has_finished_with_deadlock_detected();
        deadlock_solver_has_begun();
        recover_from_deadlock();
    }else{
        log_deadlock_detection_algorithm_has_finished_with_no_deadlock_detected();
    }
}

void free_deadlock_detection_and_recovery_algorithm(){
    free_deadlock_solver();
}