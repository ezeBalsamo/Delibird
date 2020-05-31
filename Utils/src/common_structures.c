#include <common_structures.h>
#include <stdlib.h>

void free_request(t_request* self){
    self -> sanitizer_function (self -> structure);
    free(self);
}

void free_identified_message(t_identified_message* identified_message){
    t_request* internal_request = identified_message -> request;
    free_request(internal_request);
    free(identified_message);
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
void free_localized_pokemon(t_localized_pokemon* localized_pokemon){
    list_destroy_and_destroy_elements(localized_pokemon->positions,free);
    free(localized_pokemon);
}