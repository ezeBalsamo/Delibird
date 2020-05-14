#include "../include/map.h"
#include "../include/team_manager.h"
#include "../../Utils/include/matrix.h"
#include <commons/string.h>
#include <stdlib.h>
#include <stdio.h>

t_matrix* map;

void matrix_print_trainer(void* trainer){
    char* printable_trainer = string_new();
    uint32_t trainer_sequential_number = ((t_trainer*) trainer) -> sequential_number;
    string_append(&printable_trainer, "E");
    string_append(&printable_trainer, string_itoa(trainer_sequential_number));

    printf("%10s", printable_trainer);
    free(printable_trainer);
}

uint32_t furthest_trainer_position(){
    uint32_t null_position = 0;

    void* _max_between(void* number, void* another_number){
        uint32_t cast_number =  *((uint32_t*) number);
        uint32_t cast_another_number = *((uint32_t*) another_number);
        return cast_number > cast_another_number ? number : another_number;
    }

    t_list* x_positions = trainers_x_positions();
    void* max_x_position = list_fold(x_positions, (void *) &null_position, _max_between);

    t_list* y_positions = trainers_y_positions();
    void* max_y_position = list_fold(y_positions, (void *) &null_position, _max_between);


    uint32_t furthest_position = *((uint32_t*) _max_between(max_x_position, max_y_position));

    list_destroy(x_positions);
    list_destroy(y_positions);

    return furthest_position;
}

void load_in_matrix(t_trainer* trainer){
    insert_matrix_element_at(map, trainer, trainer -> pos_x, trainer -> pos_y);
}

void initialize_map(){
    uint32_t map_size = furthest_trainer_position();
    map = matrix_create_of_size(map_size, true, false);

    with_trainers_do(load_in_matrix);
}