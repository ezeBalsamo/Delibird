#include "../include/common_structures.h"
#include <stdlib.h>

void free_request(t_request* request){
    free(request -> structure);
    free(request);
}

void free_connection_request(t_connection_request* connection_request){
    free(connection_request -> serialized_request);
    free(connection_request);
}

void free_char_array(char** char_array){
    //itero hasta que lo que apunta char array sea null, en cuyo caso null evalua como false
    for (int i = 0; char_array[i]; i++ ){
        free( char_array[i] );
    }
    free( char_array );
}

void free_serialization_information(t_serialization_information* serialization_information){
    free(serialization_information -> serialized_request);
    free(serialization_information);
}