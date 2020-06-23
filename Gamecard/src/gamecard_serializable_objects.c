#include <commons/collections/list.h>
#include "../include/gamecard_serializable_objects.h"
#include "../../Utils/include/serializable_objects.h"

t_list* gamecard_operations_information;

void initialize_gamecard_serializable_objects(){
    initialize_serializable_objects();
    gamecard_operations_information = list_create();
    list_add(gamecard_operations_information, (void*) serializable_new_pokemon());
    list_add(gamecard_operations_information, (void*) serializable_catch_pokemon());
    list_add(gamecard_operations_information, (void*) serializable_get_pokemon());
}

void free_gamecard_serializable_objects(){
    list_destroy(gamecard_operations_information);
    free_serializable_objects();
}
