#include <trainer_thread_context_execution_cycle.h>
#include "../include/movement_action.h"
#include "../../Utils/include/common_structures.h"

void approach_at_x(t_localizable_object* localizable_trainer, t_localizable_object* destiny_object){

    while(localizable_trainer -> pos_x != destiny_object -> pos_x){
        localizable_trainer -> pos_x < destiny_object -> pos_x ? localizable_trainer -> pos_x++ : localizable_trainer -> pos_x--;
        //TODO execution_cycle_consumed();
    }
}

void approach_at_y(t_localizable_object* localizable_trainer, t_localizable_object* destiny_object){

    while(localizable_trainer -> pos_y != destiny_object -> pos_y){
        localizable_trainer -> pos_y < destiny_object -> pos_y ? localizable_trainer -> pos_y++ : localizable_trainer -> pos_y--;
        //TODO execution_cycle_consumed();
    }
}

void movement_action_execution_function(t_trainer_thread_context* trainer_thread_context){

    t_movement_action* thread_action = trainer_thread_context -> thread_action;
    approach_at_x(thread_action -> localizable_trainer, thread_action -> destiny_object);
    approach_at_y(thread_action -> localizable_trainer, thread_action -> destiny_object);

    movement_action_completed_by(trainer_thread_context);
}

t_movement_action* movement_action_for(t_localizable_object* localizable_trainer, t_localizable_object* destiny_object){

    t_movement_action* thread_action = safe_malloc(sizeof(t_movement_action));
    thread_action -> action_type_description = "MOVING";
    thread_action -> localizable_trainer = localizable_trainer;
    thread_action -> destiny_object = destiny_object;

    return thread_action;
}