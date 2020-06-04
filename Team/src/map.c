#include "../include/map.h"
#include "../../Utils/include/matrix.h"
#include "../../Utils/include/common_structures.h"
#include <commons/string.h>
#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/dictionary.h>
#include <map_update_trigger.h>

t_matrix* map;
t_dictionary* pokemon_ocurrences;

void update_occurrences_of(char* pokemon_name){

    if (!dictionary_has_key(pokemon_ocurrences, pokemon_name)){
        uint32_t* occurrence = safe_malloc(sizeof(uint32_t));
        *occurrence = 0;
        dictionary_put(pokemon_ocurrences, pokemon_name, (void*) occurrence);
    }

    uint32_t* ocurrences = (uint32_t*) dictionary_get(pokemon_ocurrences, pokemon_name);
    (*ocurrences)++;
}

void matrix_print_trainer(void* trainer){
    char* printable_trainer = string_new();
    uint32_t trainer_sequential_number = ((t_trainer*) trainer) -> sequential_number;
    string_append(&printable_trainer, "E");
    string_append(&printable_trainer, string_itoa(trainer_sequential_number));

    printf("%10s", printable_trainer);
    free(printable_trainer);
}

void* cast_uint_max_between(void* number, void* another_number){
    uint32_t cast_number =  *((uint32_t*) number);
    uint32_t cast_another_number = *((uint32_t*) another_number);
    return cast_number > cast_another_number ? number : another_number;
}

uint32_t furthest_trainer_position(){
    uint32_t null_position = 0;

    t_list* x_positions = trainers_x_positions();
    void* max_x_position = list_fold(x_positions, (void *) &null_position, cast_uint_max_between);

    t_list* y_positions = trainers_y_positions();
    void* max_y_position = list_fold(y_positions, (void *) &null_position, cast_uint_max_between);

    uint32_t furthest_position = *((uint32_t*) cast_uint_max_between(max_x_position, max_y_position));

    list_destroy(x_positions);
    list_destroy(y_positions);

    return furthest_position;
}

void load_trainer_in_map(t_localizable_object* localizable_trainer){
    insert_matrix_element_at(map, (t_trainer*) localizable_trainer -> object, localizable_trainer -> pos_x, localizable_trainer -> pos_y);
}
void load_pokemon_in_map(t_localizable_object* localized_object){

    char* pokemon_name = localized_object -> object;

    insert_matrix_element_at(map, pokemon_name, localized_object -> pos_x, localized_object -> pos_y);
    update_occurrences_of(pokemon_name);
    map_updated_with_insertion_of(localized_object);
}

void remove_pokemon_from_matrix(t_localizable_object* t_localizable_object){
    //TODO
}
void initialize_map(){
    uint32_t map_size = furthest_trainer_position();
    map = matrix_create_of_size(map_size, true, false);
    pokemon_ocurrences = dictionary_create();
    with_trainers_do(load_trainer_in_map);
}