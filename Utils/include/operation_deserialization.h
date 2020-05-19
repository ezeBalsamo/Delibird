#ifndef DELIBIRD_OPERATION_DESERIALIZATION_H
#define DELIBIRD_OPERATION_DESERIALIZATION_H

#include "common_structures.h"

void* deserialize_appeared_pokemon(void* serialized_structure);
void* deserialize_new_pokemon(void* serialized_structure);
void* deserialize_catch_pokemon(void* serialized_structure);
void* deserialize_caught_pokemon(void* serialized_structure);
void* deserialize_get_pokemon(void* serialized_structure);
void* deserialize_subscribe_me(void* serialized_structure);
void* deserialize_localized_pokemon(void* serialized_structure);

#endif //DELIBIRD_OPERATION_DESERIALIZATION_H
