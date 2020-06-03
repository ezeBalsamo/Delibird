#include <matrix_update_trigger.h>
#include <dispatcher.h>
#include <team_logs_manager.h>
#include <trainer_threads.h>
#include "../../Utils/include/free_system.h"
#include <stdlib.h>

void assert_not_empty(t_list* trainer_threads){

    if(list_is_empty(trainer_threads)){
        log_zero_schedulable_threads_error();
        free_system();
    }
}

uint32_t distance_between(t_trainer* trainer, t_pokemon* pokemon){
    uint32_t x_distance = abs(trainer -> pos_x - pokemon -> pos_x);
    uint32_t y_distance = abs(trainer -> pos_y - pokemon -> pos_y);

    return x_distance + y_distance;
}

void* closest_trainer_to(t_synchronizable_trainer* synchronizable_trainer,
                         t_synchronizable_trainer* another_synchronizable_trainer, t_pokemon* pokemon){

    uint32_t trainer_distance = distance_between(synchronizable_trainer -> trainer, pokemon);
    uint32_t another_trainer_distance = distance_between(another_synchronizable_trainer -> trainer, pokemon);

    return trainer_distance < another_trainer_distance ? synchronizable_trainer : another_synchronizable_trainer;
}

void matrix_updated_with_insertion_of(t_pokemon* pokemon){
    t_list* synchronizable_trainers = schedulable_synchronizable_trainers();

    assert_not_empty(synchronizable_trainers);
    t_synchronizable_trainer* first_synchronizable_trainer = list_get(synchronizable_trainers, 0);

    void* _closest_trainer(void* synchronizable_trainer, void* another_synchronizable_trainer){
        t_synchronizable_trainer* cast_synchronizable_trainer = (t_synchronizable_trainer*) synchronizable_trainer;
        t_synchronizable_trainer* cast_another_synchronizable_trainer = (t_synchronizable_trainer*) another_synchronizable_trainer;

        return closest_trainer_to(cast_synchronizable_trainer, cast_another_synchronizable_trainer, pokemon);
    }

    t_synchronizable_trainer* synchronizable_trainer = (t_synchronizable_trainer*)
            list_fold(synchronizable_trainers, first_synchronizable_trainer, _closest_trainer);

    trainer_ready_to_be_sheduled(synchronizable_trainer);
}