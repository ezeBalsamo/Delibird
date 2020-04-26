#ifndef SERIALIZATION_INTERFACE_H
#define SERIALIZATION_INTERFACE_H

#include "common_structures.h"

void serialize(t_request* request, void** serialized_structure, uint32_t* amount_of_bytes);

void* deserialize(void* serialized_request);

#endif //SERIALIZATION_INTERFACE_H
