#ifndef DELIBIRD_OPERATION_SERIALIZATION_H
#define DELIBIRD_OPERATION_SERIALIZATION_H

#include "common_structures.h"

t_serialization_information* serialize(t_request* request);
t_serialization_information* serialize_appeared_pokemon(void* structure);
t_serialization_information* serialize_new_pokemon(void* structure);
t_serialization_information* serialize_catch_pokemon(void* structure);
t_serialization_information* serialize_caught_pokemon(void* structure);
t_serialization_information* serialize_get_pokemon(void* structure);
t_serialization_information* serialize_suscribe_me(void* structure);

#endif //DELIBIRD_OPERATION_SERIALIZATION_H