#include <trainer_thread_context_execution_cycle.h>
#include <team_logs_manager.h>
#include "../include/movement_action.h"
#include <stdlib.h>
#include <event_notifier.h>

void approach_at_x(t_localizable_object* localizable_trainer, t_localizable_object* destiny_object){

    while(localizable_trainer -> pos_x != destiny_object -> pos_x){
        localizable_trainer -> pos_x < destiny_object -> pos_x ? localizable_trainer -> pos_x++ : localizable_trainer -> pos_x--;
        log_trainer_movement(localizable_trainer);
        notify_with_argument(EXECUTION_CYCLE_CONSUMED_BY_TRAINER, localizable_trainer -> object);
    }
}

void approach_at_y(t_localizable_object* localizable_trainer, t_localizable_object* destiny_object){

    while(localizable_trainer -> pos_y != destiny_object -> pos_y){
        localizable_trainer -> pos_y < destiny_object -> pos_y ? localizable_trainer -> pos_y++ : localizable_trainer -> pos_y--;
        log_trainer_movement(localizable_trainer);
        notify_with_argument(EXECUTION_CYCLE_CONSUMED_BY_TRAINER, localizable_trainer -> object);
    }
}

void movement_action_execution_function(t_trainer_thread_context* trainer_thread_context){

    t_movement_action* thread_action = internal_thread_action_in(trainer_thread_context);
    approach_at_x(thread_action -> localizable_trainer, thread_action -> destiny_object);
    approach_at_y(thread_action -> localizable_trainer, thread_action -> destiny_object);

    movement_action_completed_by(trainer_thread_context);
}

t_thread_action* movement_thread_action_for(t_localizable_object* localizable_trainer, t_localizable_object* destiny_object){

    t_movement_action* movement_action = safe_malloc(sizeof(t_movement_action));
    movement_action -> localizable_trainer = localizable_trainer;
    movement_action -> destiny_object = destiny_object;

    t_thread_action* thread_action = new_thread_action();
    thread_action -> request -> operation = MOVEMENT;
    thread_action -> request -> structure = movement_action;
    thread_action -> request -> sanitizer_function = free;
    thread_action -> execution_function = (void (*)(void *)) movement_action_execution_function;

    return thread_action;
}