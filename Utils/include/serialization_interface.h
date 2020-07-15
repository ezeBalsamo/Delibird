#ifndef SERIALIZATION_INTERFACE_H
#define SERIALIZATION_INTERFACE_H

#include <stdint.h>

typedef struct Request{
    uint32_t operation;
    void* structure;
    void (*sanitizer_function) (void* request_structure);
}t_request;

typedef struct Serialization_information{
    void* serialized_request;
    uint32_t amount_of_bytes;
}t_serialization_information;

typedef struct Deserialization_information{
    uint32_t operation;
    void* serialized_structure;
    uint32_t serialized_structure_size;
}t_deserialization_information;

typedef struct Serializable_object{
    uint32_t code;
    t_serialization_information* (*serialize_function) (void* structure);
    t_request* (*deserialize_function) (void* serialized_structure);
}t_serializable_object;

t_serialization_information* serialize(t_request* request);

t_request* deserialize(void* serialized_request);
t_request* deserialize_with_deserialization_information(t_deserialization_information* deserialization_information);

void free_request(t_request* request);
void free_serialization_information(t_serialization_information* serialization_information);
void free_deserialization_information(t_deserialization_information* deserialization_information);

#endif //SERIALIZATION_INTERFACE_H
