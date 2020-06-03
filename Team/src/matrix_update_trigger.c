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

void* closest_trainer_to(t_synchronizable_trainer* trainer_with_lock,
                         t_synchronizable_trainer* another_trainer_with_lock, t_pokemon* pokemon){

    uint32_t trainer_distance = distance_between(trainer_with_lock -> trainer, pokemon);
    uint32_t another_trainer_distance = distance_between(another_trainer_with_lock -> trainer, pokemon);

    return trainer_distance < another_trainer_distance ? trainer_with_lock : another_trainer_with_lock;
}

void matrix_updated_with_insertion_of(t_pokemon* pokemon){
    t_list* trainer_threads = schedulable_threads();

    assert_not_empty(trainer_threads);
    t_synchronizable_trainer* first_trainer_thread = list_get(trainer_threads, 0);

    void* _closest_trainer(void* trainer_with_lock, void* another_trainer_with_lock){
        t_synchronizable_trainer* cast_trainer_with_lock = (t_synchronizable_trainer*) trainer_with_lock;
        t_synchronizable_trainer* cast_another_trainer_with_lock = (t_synchronizable_trainer*) another_trainer_with_lock;

        return closest_trainer_to(cast_trainer_with_lock, cast_another_trainer_with_lock, pokemon);
    }

    t_synchronizable_trainer* trainer_thread = (t_synchronizable_trainer*)
            list_fold(trainer_threads, first_trainer_thread, _closest_trainer);

    trainer_ready_to_be_sheduled(trainer_thread);
}