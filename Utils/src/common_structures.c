#include "../include/common_structures.h"
#include <stdlib.h>

void free_request(t_request* request){
    free(request->structure);
    free(request);
}

void free_process_information(t_process_information* process_information){
    //TODO
}

void free_operation_information(t_operation_information* operation_information){
    //TODO
}
void free_char_array(char** char_array){
    //TODO
}

void free_serialization_information(t_serialization_information* serialization_information){
    free(serialization_information -> serialized_request);
    free(serialization_information);
}