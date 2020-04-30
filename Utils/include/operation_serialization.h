#ifndef DELIBIRD_OPERATION_SERIALIZATION_H
#define DELIBIRD_OPERATION_SERIALIZATION_H

#include "common_structures.h"

t_serialization_information* serialize(t_request* request);
t_serialization_information* serialize_appeared_pokemon(char** arguments);
t_serialization_information* serialize_new_pokemon(char** arguments);
t_serialization_information* serialize_catch_pokemon(char** arguments);
t_serialization_information* serialize_caught_pokemon(char** arguments);
t_serialization_information* serialize_get_pokemon(char** arguments);

#endif //DELIBIRD_OPERATION_SERIALIZATION_H