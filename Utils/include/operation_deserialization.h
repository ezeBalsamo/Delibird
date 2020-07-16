#ifndef DELIBIRD_OPERATION_DESERIALIZATION_H
#define DELIBIRD_OPERATION_DESERIALIZATION_H

#include "common_structures.h"
#include "serialization_interface.h"

t_request* deserialize_appeared_pokemon(void* serialized_structure);
t_request* deserialize_new_pokemon(void* serialized_structure);
t_request* deserialize_catch_pokemon(void* serialized_structure);
t_request* deserialize_caught_pokemon(void* serialized_structure);
t_request* deserialize_get_pokemon(void* serialized_structure);
t_request* deserialize_subscribe_me(void* serialized_structure);
t_request* deserialize_localized_pokemon(void* serialized_structure);
t_request* deserialize_identified_message(void* serialized_structure);

t_deserialization_information* deserialization_information_of(void* serialized_request);

#endif //DELIBIRD_OPERATION_DESERIALIZATION_H
