#ifndef DELIBIRD_OPERATION_DESERIALIZATION_H
#define DELIBIRD_OPERATION_DESERIALIZATION_H

#include "common_structures.h"

t_printable_object* deserialize_appeared_pokemon(void* serialized_structure);
t_printable_object* deserialize_new_pokemon(void* serialized_structure);
t_printable_object* deserialize_catch_pokemon(void* serialized_structure);
t_printable_object* deserialize_caught_pokemon(void* serialized_structure);
t_printable_object* deserialize_get_pokemon(void* serialized_structure);
t_printable_object* deserialize_subscribe_me(void* serialized_structure);
t_printable_object* deserialize_localized_pokemon(void* serialized_structure);

#endif //DELIBIRD_OPERATION_DESERIALIZATION_H
