#include <serialization_interface.h>
#include <stdlib.h>

void free_request(t_request* self){
    self -> sanitizer_function (self -> structure);
    free(self);
}

void free_serialization_information(t_serialization_information* serialization_information){
    free(serialization_information -> serialized_request);
    free(serialization_information);
}

void free_deserialization_information(t_deserialization_information* deserialization_information){
    free(deserialization_information -> serialized_structure);
    free(deserialization_information);
}