#ifndef DELIBIRD_OPERATION_DESERIALIZATION_H
#define DELIBIRD_OPERATION_DESERIALIZATION_H

#include "common_structures.h"

t_request* deserialize_appeared_pokemon(void* serialized_structure);
t_request* deserialize_new_pokemon(void* serialized_structure);
t_request* deserialize_catch_pokemon(void* serialized_structure);
t_request* deserialize_caught_pokemon(void* serialized_structure);
t_request* deserialize_get_pokemon(void* serialized_structure);
t_request* deserialize_suscribe_me(void* serialized_structure);
t_request* deserialize_localized_pokemon(void* serialized_structure);

#endif //DELIBIRD_OPERATION_DESERIALIZATION_H
