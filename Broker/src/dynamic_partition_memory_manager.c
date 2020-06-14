#include <broker_memory_manager.h>
#include <string.h>
#include "../../Utils/include/operation_serialization.h"
#include "../../Utils/include/configuration_manager.h"

t_memory_manager* dynamic_partition_memory_manager;

void dynamic_partition_allocate_message(t_identified_message* message){
    //logica para guardar un mensaje en memoria


    //  PASO 1 : OBTENER EL MENSAJE
    t_request* message_request = message->request;

    uint32_t operation = internal_operation_in(message);
    if(operation == IDENTIFIED_MESSAGE){
        message_request = (t_request*) internal_request_in_correlative(message);
    }
    //instancio un bloque de memoria, ingresando el espacio que va a ocupar su mensaje
    t_memory_block* new_memory_block = safe_malloc(sizeof(t_memory_block));
    t_serialization_information* request_serialized = serialize(message_request);
    new_memory_block->message_size = request_serialized->amount_of_bytes;
    //lru value
    //id bloque

    uint32_t search_failed_count = 0;

    t_block_manager* block_manager_found;  //para referenciarlo afuera
    while(search_failed_count <= dynamic_partition_memory_manager->message_allocator->max_search_tries){

        block_manager_found =  dynamic_partition_memory_manager->message_allocator->find_available_partition_algorithm (new_memory_block->message_size);
        if (block_manager_found != NULL){
            break;
        }
        dynamic_partition_memory_manager->free_partition_algorithm ();
        search_failed_count++;

        if (search_failed_count > dynamic_partition_memory_manager->max_search_tries){
            dynamic_partition_memory_manager->compact_memory_function();
            search_failed_count=0;
        }
    }
    //encontre un block manager que tiene lugar, ahora tengo que particionarlo con la cantidad que necesito (> min)

}



void* get_dynamic_partition_allocate_message_algorithm(){

    return dynamic_partition_allocate_message;
}

