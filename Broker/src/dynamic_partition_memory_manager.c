#include <broker_memory_manager.h>
#include <string.h>
#include "../../Utils/include/operation_serialization.h"
#include "../../Utils/include/configuration_manager.h"

t_memory_manager* dynamic_partition_memory_manager;

uint32_t size_to_allocate_for(t_request* message_request){
    uint32_t size = 0;
    switch (message_request->operation) {
        case NEW_POKEMON:{
            t_new_pokemon* new_pokemon = (t_new_pokemon*) message_request->structure;
            return amount_of_bytes_of_new(new_pokemon);
        }
        case LOCALIZED_POKEMON:{
            t_localized_pokemon* localized_pokemon = (t_localized_pokemon*) message_request->structure;
            return amount_of_bytes_of_localized(localized_pokemon);
        }
        case GET_POKEMON:{
            t_get_pokemon* get_pokemon = (t_get_pokemon*) message_request->structure;
            return amount_of_bytes_of_get(get_pokemon);
        }
        case APPEARED_POKEMON:{
            t_appeared_pokemon* appeared_pokemon = (t_appeared_pokemon*) message_request->structure;
            return amount_of_bytes_of_appeared(appeared_pokemon);
        }
        case CATCH_POKEMON:{
            t_catch_pokemon* catch_pokemon = (t_catch_pokemon*) message_request->structure;
            return amount_of_bytes_of_catch(catch_pokemon);
        }
        case CAUGHT_POKEMON:{
            return sizeof(uint32_t); //caught status 0-1
        }
    }
    return size;
}
void dynamic_partition_allocate_message_function(t_identified_message* message){
    //logica para guardar un mensaje en memoria


    //  PASO 1 : OBTENER EL MENSAJE
    t_request* message_request = message->request;

    uint32_t operation = internal_operation_in(message);
    if(operation == IDENTIFIED_MESSAGE){
        message_request = (t_request*) internal_request_in_correlative(message);
    }

    t_memory_block* new_memory_block = safe_malloc(sizeof(t_memory_block));
    new_memory_block->message_size = size_to_allocate_for(message_request);
    uint32_t search_failed_count = 0;

    t_block_manager* block_manager_found;
    while(search_failed_count <= dynamic_partition_memory_manager->max_search_tries){
        block_manager_found =  dynamic_partition_memory_manager->find_available_partition_algorithm (new_memory_block->message_size);
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
    //

}

t_memory_manager* initialize_dynamic_partition_memory_manager(){

    dynamic_partition_memory_manager = safe_malloc(sizeof(t_memory_manager));
    dynamic_partition_memory_manager->blocks_manager = list_create();
    dynamic_partition_memory_manager->allocate_message_function = dynamic_partition_allocate_message_function;

    uint32_t memory_size = config_get_int_at("TAMANO_MEMORIA");
    //initial block manager, with all the memory
    t_block_manager* initial_block_manager = safe_malloc(sizeof(t_block_manager));
    initial_block_manager->block_size = memory_size;
    initial_block_manager->free_block = true;
    initial_block_manager->initial_position = safe_malloc(memory_size);
    initial_block_manager->memory_block = NULL;

    list_add(dynamic_partition_memory_manager->blocks_manager,(void*) initial_block_manager);

    return dynamic_partition_memory_manager;
}

