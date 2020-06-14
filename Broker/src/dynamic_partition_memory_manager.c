#include <broker_memory_manager.h>
#include <string.h>
#include <broker_memory_algorithms.h>
#include "../../Utils/include/operation_serialization.h"
#include "../../Utils/include/configuration_manager.h"

t_message_allocator* dynamic_partition_message_allocator;

t_message_allocator* initialize_dynamic_partition_message_allocator(){
    dynamic_partition_message_allocator = safe_malloc(sizeof(t_message_allocator));
    dynamic_partition_message_allocator->allocate_message_function = get_allocate_message_algorithm(); //CASTEAR ESTO?
    dynamic_partition_message_allocator->find_available_partition_algorithm = get_search_partition_algorithm();
    dynamic_partition_message_allocator->free_partition_algorithm = get_free_partition_algorithm();
    dynamic_partition_message_allocator->compact_memory_algorithm = compact_memory_algorithm;
    dynamic_partition_message_allocator->min_partition_size = config_get_int_at("TAMANO_MINIMO_PARTICION");
    dynamic_partition_message_allocator->max_search_tries = config_get_int_at("FRECUENCIA_COMPACTACION");
    return dynamic_partition_message_allocator;
}

void dynamic_partition_allocate_message(t_identified_message* message,t_list* memory_manager){
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
    while(search_failed_count <= dynamic_partition_message_allocator->max_search_tries){

        block_manager_found = dynamic_partition_message_allocator->find_available_partition_algorithm (new_memory_block->message_size);
        if (block_manager_found != NULL){
            break;
        }
        dynamic_partition_message_allocator->free_partition_algorithm ();
        search_failed_count++;

        if (search_failed_count > dynamic_partition_message_allocator->max_search_tries){
            dynamic_partition_message_allocator->compact_memory_algorithm();
            search_failed_count=0;
        }
    }
    //encontre un block manager que tiene lugar, ahora tengo que particionarlo con la cantidad que necesito (> min)

}



void* get_dynamic_partition_allocate_message_algorithm(){

    return dynamic_partition_allocate_message;
}

