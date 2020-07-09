#include <broker_memory_manager.h>
#include <string.h>
#include <broker_memory_algorithms.h>
#include <broker_logs_manager.h>
#include "../../Utils/include/operation_serialization.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/pokemon_request_bytes_calculator.h"
#include "../../Utils/include/garbage_collector.h"

t_message_allocator* dynamic_partition_message_allocator;


t_block_information*  find_block_to_allocate_message(t_list* blocks_information, t_memory_block* memory_block_to_save){
    uint32_t number_of_partitions_freed = 0;
    while(true){

        t_block_information* block_information_found = dynamic_partition_message_allocator->available_partition_search_algorithm (memory_block_to_save->message_size, blocks_information, dynamic_partition_message_allocator->min_partition_size);
        if (block_information_found != NULL){
            return block_information_found;
        }

        //todo: semaforizar
        if (number_of_partitions_freed >= dynamic_partition_message_allocator->max_search_tries){
            int amount_of_partitions_before_compaction = list_size(blocks_information);

            dynamic_partition_message_allocator->memory_compaction_algorithm(blocks_information);

            int amount_of_partitions_compacted = list_size(blocks_information) - amount_of_partitions_before_compaction;
            number_of_partitions_freed=0;

            log_succesful_memory_compaction(amount_of_partitions_compacted);

            block_information_found = dynamic_partition_message_allocator->available_partition_search_algorithm (memory_block_to_save->message_size, blocks_information, dynamic_partition_message_allocator->min_partition_size);
            if (block_information_found != NULL){
                return block_information_found;
            }
        }
        void* partition_freed_position = dynamic_partition_message_allocator->partition_free_algorithm (blocks_information);
        log_succesful_free_partition_to_cache(partition_freed_position);

        number_of_partitions_freed++;
    }
}

uint32_t block_size_for(t_memory_block *  memory_block_to_save){
    bool block_to_save_bigger_than_min_partition_size = memory_block_to_save->message_size > dynamic_partition_message_allocator->min_partition_size;

    return block_to_save_bigger_than_min_partition_size ? memory_block_to_save->message_size : dynamic_partition_message_allocator->min_partition_size;
}

t_block_information* save_memory_block_in_block_information(t_block_information* block_information_found,t_memory_block* memory_block_to_save){
    uint32_t memory_size_to_partition = block_information_found->block_size;
    uint32_t block_size_to_allocate = block_size_for(memory_block_to_save);

    block_information_found->is_free = false;
    block_information_found->memory_block = memory_block_to_save;
    block_information_found->block_size = block_size_to_allocate;

    uint32_t memory_size_left = memory_size_to_partition - block_information_found->block_size;

    t_block_information* new_block_information = NULL;
    if (memory_size_left > 0){
        new_block_information = safe_malloc(sizeof(t_block_information));

        new_block_information->is_free = true;
        new_block_information->initial_position = block_information_found->initial_position + block_size_to_allocate;
        new_block_information->block_size = memory_size_left;
    }

    return new_block_information;
}

t_request* message_request_from_identified_message(t_identified_message* message){
    uint32_t operation = internal_operation_in(message);
    return operation == IDENTIFIED_MESSAGE ?  (t_request *) internal_request_in_correlative(message) : message->request;
}

t_memory_block* build_memory_block_from_message(t_identified_message* message) {
    //Obtengo el mensaje
    t_request* message_request = message_request_from_identified_message(message);

    t_memory_block *memory_block_to_save = safe_malloc(sizeof(t_memory_block));

    memory_block_to_save->message_id =  message->message_id;
    memory_block_to_save->message_operation = message_request->operation;

    t_serialization_information *request_serialized = serialize(message_request);

    memory_block_to_save->message_size = size_to_allocate_for(message_request);
    memory_block_to_save->message = ((t_request *) request_serialized->serialized_request)->structure;
    memory_block_to_save->lru_value = current_time_in_milliseconds();
    if(memory_block_to_save->message_size > dynamic_partition_message_allocator->max_partition_size){
        log_invalid_operation_to_save_message_error();
        free_system();
    }
    return memory_block_to_save;
}
int block_index_position(t_block_information* block_to_find,t_list* blocks_information){
    int block_index = 0;
    for(int i= 0; i < list_size(blocks_information); i++){
        t_block_information* block_found = (t_block_information*) list_get(blocks_information,i);
        if(block_found->initial_position == block_to_find->initial_position){
            return i;
        }
    }
    free_system();
    return -1;
}

//ACA ARRANCA LA PAPA
void partition_allocate_message(t_identified_message* message,t_list* blocks_information){
    //logica para guardar un mensaje en memoria

    t_memory_block* memory_block_to_save = build_memory_block_from_message(message);

    t_block_information* block_information_found = find_block_to_allocate_message(blocks_information, memory_block_to_save);

    //encontre un block manager disponible, lo spliteo y creo uno nuevo que tenga la memoria restante, que este libre
    t_block_information* new_block_information = save_memory_block_in_block_information(block_information_found,memory_block_to_save);
    // es posible que no haya que crear uno nuevo, si la particion tenia el tamaño exacto necesario
    if (new_block_information != NULL){
        int position = block_index_position(block_information_found,blocks_information);
        list_add_in_index(blocks_information,position+1,(void*) new_block_information);
    }

    log_succesful_save_message_to_cache(message_request_from_identified_message(message),new_block_information->initial_position);
}

t_message_allocator* initialize_dynamic_partition_message_allocator(){

    dynamic_partition_message_allocator = safe_malloc(sizeof(t_message_allocator));
    dynamic_partition_message_allocator->allocate_message_function = partition_allocate_message;

    dynamic_partition_message_allocator->min_partition_size = config_get_int_at("TAMANO_MINIMO_PARTICION");
    dynamic_partition_message_allocator->max_search_tries = config_get_int_at("FRECUENCIA_COMPACTACION");
    dynamic_partition_message_allocator->max_partition_size = config_get_int_at("TAMANO_MEMORIA");

    dynamic_partition_message_allocator->available_partition_search_algorithm = get_available_partition_search_algorithm(); //FF/BF
    dynamic_partition_message_allocator->partition_free_algorithm = get_partition_free_algorithm(); //FIFO/LRU
    dynamic_partition_message_allocator->memory_compaction_algorithm = memory_compaction_algorithm;

    return dynamic_partition_message_allocator;
}

