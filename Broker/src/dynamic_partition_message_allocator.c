#include <broker_memory_manager.h>
#include <string.h>
#include <broker_memory_algorithms.h>
#include "../../Utils/include/operation_serialization.h"
#include "../../Utils/include/configuration_manager.h"

t_message_allocator* dynamic_partition_message_allocator;

void dynamic_partition_allocate_message(t_identified_message* message,t_list* blocks_manager){
    //logica para guardar un mensaje en memoria

    //Obtengo el mensaje
    t_request* message_request = message->request;
    uint32_t message_id = message->message_id;
    uint32_t operation = internal_operation_in(message);

    if(operation == IDENTIFIED_MESSAGE){
        message_id = ((t_identified_message*)message->request->structure) -> message_id;
        message_request = (t_request*) internal_request_in_correlative(message);
    }
    //instancio un bloque de memoria, ingresando el espacio que va a ocupar su mensaje
    t_memory_block* memory_block_to_save = safe_malloc(sizeof(t_memory_block));

    memory_block_to_save->message_id = message_id;
    memory_block_to_save->message_operation = message_request->operation;

    t_serialization_information* request_serialized = serialize(message_request); //TODO: OJO!!

    memory_block_to_save->message_size = size_to_allocate_for(message_request); //request_serialized->amount_of_bytes incluye bytes que no quiero
    memory_block_to_save->message = ((t_request*) request_serialized->serialized_request) -> structure;
    //lru value
    //id bloque

    uint32_t search_failed_count = 0;

    t_block_manager* block_manager_found;  //para referenciarlo afuera
    while(search_failed_count <= dynamic_partition_message_allocator->max_search_tries){

        block_manager_found = dynamic_partition_message_allocator->find_available_partition_algorithm (memory_block_to_save->message_size, blocks_manager);
        if (block_manager_found != NULL){
            break;
        }
        dynamic_partition_message_allocator->free_partition_algorithm (blocks_manager);

        block_manager_found = dynamic_partition_message_allocator->find_available_partition_algorithm (memory_block_to_save->message_size, blocks_manager);
        if (block_manager_found != NULL){
            break;
        }

        search_failed_count++;

        if (search_failed_count > dynamic_partition_message_allocator->max_search_tries){
            dynamic_partition_message_allocator->compact_memory_algorithm(blocks_manager);
            search_failed_count=0;
        }
    }

    //encontre un block manager disponible, ahora tengo que particionarlo con la cantidad que necesito (> min)
    //creando uno nuevo que tenga la memoria restante, que este libre
    t_block_manager* new_block_manager = safe_malloc(sizeof(t_block_manager));
    new_block_manager->free_block = true;
    new_block_manager->block_size = block_manager_found->block_size - memory_block_to_save->message_size;
    new_block_manager->initial_position = block_manager_found->initial_position + new_block_manager->block_size;

    block_manager_found->free_block = false;
    block_manager_found->memory_block = memory_block_to_save;
    block_manager_found->block_size = dynamic_partition_message_allocator->min_partition_size;

    if(memory_block_to_save->message_size > dynamic_partition_message_allocator->min_partition_size){
        block_manager_found->block_size = memory_block_to_save->message_size;
    }

    new_block_manager->initial_position = block_manager_found->initial_position + block_manager_found->block_size;

    list_add(blocks_manager,(void*) new_block_manager);
}

t_message_allocator* initialize_dynamic_partition_message_allocator(){

    dynamic_partition_message_allocator = safe_malloc(sizeof(t_message_allocator));
    dynamic_partition_message_allocator->allocate_message_function = dynamic_partition_allocate_message; //CASTEAR ESTO?

    dynamic_partition_message_allocator->find_available_partition_algorithm = get_search_partition_algorithm(); //FF/BF
    dynamic_partition_message_allocator->free_partition_algorithm = get_free_partition_algorithm(); //FIFO/LRU
    dynamic_partition_message_allocator->compact_memory_algorithm = compact_memory_algorithm;
    dynamic_partition_message_allocator->min_partition_size = config_get_int_at("TAMANO_MINIMO_PARTICION");
    dynamic_partition_message_allocator->max_search_tries = config_get_int_at("FRECUENCIA_COMPACTACION");

    return dynamic_partition_message_allocator;
}

