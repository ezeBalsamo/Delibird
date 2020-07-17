#include "../include/deserialization_information_content_provider.h"
#include "../../Utils/include/common_structures.h"
#include <string.h>


uint32_t internal_operation_queue_using(t_deserialization_information* deserialization_information){

    /* 0 - uint32t - identified_message -> message_id
     * 1 - uint32t - internal_request_amount_of_bytes
     *      void*  - internal_request
     * 2 - uint32t - operation_queue
     */

    void* internal_serialized_request = deserialization_information -> serialized_structure + sizeof(uint32_t) * 2;

    uint32_t operation_queue;
    memcpy(&operation_queue, internal_serialized_request, sizeof(uint32_t));

    return operation_queue;
}

uint32_t operation_queue_using(t_deserialization_information* deserialization_information){

    uint32_t possible_operation_code = deserialization_information -> operation;

    if(possible_operation_code == IDENTIFIED_MESSAGE){
        return internal_operation_queue_using(deserialization_information);
    }

    return possible_operation_code;
}

uint32_t internal_message_size_using(t_deserialization_information* deserialization_information){

    /* 0 - uint32t - identified_message -> message_id
     * 1 - uint32t - internal_request_amount_of_bytes
     *      void*  - serialized_request
     * 2 - uint32t - internal_operation_queue
     * 3 - uint32t - internal_structure_size
     */

    uint32_t internal_message_size;

    void* internal_message_size_pointer = deserialization_information -> serialized_structure + sizeof(uint32_t) * 3;
    memcpy(&internal_message_size, internal_message_size_pointer, sizeof(uint32_t));

    return internal_message_size;
}

uint32_t message_size_using(t_deserialization_information* deserialization_information){

    if(deserialization_information -> operation == IDENTIFIED_MESSAGE){
        return internal_message_size_using(deserialization_information);
    }

    return deserialization_information -> serialized_structure_size;
}

uint32_t internal_message_id_using(t_deserialization_information* deserialization_information){

    // 0 - uint32t - identified_message -> message_id

    uint32_t correlative_message_id;
    memcpy(&correlative_message_id, deserialization_information -> serialized_structure, sizeof(uint32_t));

    return correlative_message_id;
}

uint32_t correlative_message_id_using(t_deserialization_information* deserialization_information){

    if(deserialization_information -> operation == IDENTIFIED_MESSAGE){
        return internal_message_id_using(deserialization_information);
    }

    return 0;
}

void* internal_structure_using(t_deserialization_information* deserialization_information){

    /* 0 - uint32t - identified_message -> message_id
     * 1 - uint32t - internal_request_amount_of_bytes
     *      void*  - serialized_request
     * 2 - uint32t - internal_operation_queue
     * 3 - uint32t - internal_structure_size
     *      void*  - structure
     */

    return deserialization_information -> serialized_structure + sizeof(uint32_t) * 4;
}

void store_internal_message_using(void* message_initial_position, t_deserialization_information* deserialization_information){

    void* internal_structure = internal_structure_using(deserialization_information);
    uint32_t internal_message_size = internal_message_size_using(deserialization_information);

    memcpy(message_initial_position, internal_structure, internal_message_size);
}

void store_message_using(void* message_initial_position, t_deserialization_information* deserialization_information){

    if(deserialization_information -> operation == IDENTIFIED_MESSAGE){
        store_internal_message_using(message_initial_position, deserialization_information);
    }else{
        memcpy(message_initial_position,
               deserialization_information -> serialized_structure,
               deserialization_information -> serialized_structure_size);
    }
}