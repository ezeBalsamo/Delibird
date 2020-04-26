#ifndef DELIBIRD_OPERATION_SERIALIZATION_H
#define DELIBIRD_OPERATION_SERIALIZATION_H

#include "common_structures.h"

void serialize(t_request* request , void** serialized_structure, uint32_t* amount_of_bytes);

void serialize_appeared_pokemon(char** arguments, void** serialized_structure, uint32_t* amount_of_bytes);
void serialize_new_pokemon(char** arguments, void** serialized_structure, uint32_t* amount_of_bytes);
void serialize_catch_pokemon(char** arguments, void** serialized_structure, uint32_t* amount_of_bytes);
void serialize_caught_pokemon(char** arguments, void** serialized_structure, uint32_t* amount_of_bytes);
void serialize_get_pokemon(char** arguments, void** serialized_structure, uint32_t* amount_of_bytes);

#endif //DELIBIRD_OPERATION_SERIALIZATION_H