#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "common_structures.h"

void serialize(request* request, void** serialized_structure, uint32_t* amount_of_bytes);

void* deserialize(void* serialized_request);

#endif //SERIALIZATION_H
