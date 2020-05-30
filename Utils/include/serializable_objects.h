#ifndef DELIBIRD_SERIALIZABLE_OBJECTS_H
#define DELIBIRD_SERIALIZABLE_OBJECTS_H

#include "common_structures.h"

t_serializable_object* serializable_new_pokemon();

t_serializable_object* serializable_appeared_pokemon();

t_serializable_object* serializable_catch_pokemon();

t_serializable_object* serializable_caught_pokemon();

t_serializable_object* serializable_get_pokemon();

t_serializable_object* serializable_localized_pokemon();

t_serializable_object* serializable_subscribe_me();

t_serializable_object* serializable_identified_me();

void initialize_serializable_objects();

t_serializable_object* serializable_object_with_code(uint32_t operation_code);

void free_serializable_objects();

#endif //DELIBIRD_SERIALIZABLE_OBJECTS_H
