#ifndef SERIALIZATION_INTERFACE_H
#define SERIALIZATION_INTERFACE_H

#include "common_structures.h"

t_serialization_information* serialize(t_request* request);
t_request* deserialize(void* serialized_request);

#endif //SERIALIZATION_INTERFACE_H
