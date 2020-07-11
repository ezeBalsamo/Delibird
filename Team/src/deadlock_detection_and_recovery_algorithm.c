#include <deadlock_detecter.h>
#include <deadlock_solver.h>
#include <team_logs_manager.h>
#include <dispatcher.h>
#include "deadlock_detection_and_recovery_algorithm.h"
#include "../../Utils/include/pthread_wrapper.h"

pthread_t deadlock_thread;

void initialize_deadlock_detection_and_recovery_algorithm(){
    initialize_deadlock_solver();
}

void* deadlock_solver_thread(){
    log_deadlock_detection_algorithm_has_finished_with_deadlock_detected();
    deadlock_solver_has_begun();
    recover_from_deadlock();
    deadlock_solver_has_ended();

    return NULL;
}

void detect_and_recover_from_deadlock(){
    log_deadlock_detection_algorithm_has_begun();
    if(deadlock_exists()){
//        calculate_and_notify_deadlocks_quantity();
        deadlock_thread = default_safe_thread_create(deadlock_solver_thread, NULL);
    }else{
        log_deadlock_detection_algorithm_has_finished_with_no_deadlock_detected();
    }
}

void join_deadlock_solver_thread(){
    safe_thread_join(deadlock_thread);
}

void free_deadlock_detection_and_recovery_algorithm(){
    free_deadlock_solver();
}