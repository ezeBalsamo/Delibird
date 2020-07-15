#include <broker_memory_manager.h>
#include <string.h>
#include <broker_memory_algorithms.h>
#include <broker_logs_manager.h>
#include <broker_message_allocator.h>
#include <memory_compaction_algorithm.h>
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/t_list_extension.h"

t_message_allocator* dynamic_partition_message_allocator;

void consolidate_block_for_dynamic_partition(t_list* blocks_information,int index_of_block_to_consolidate){
    t_block_information* master_block = (t_block_information*) list_get(blocks_information,index_of_block_to_consolidate);

    if (is_valid_index(blocks_information,index_of_block_to_consolidate+1)
        && is_free_block_in_index(blocks_information,index_of_block_to_consolidate+1)){ //evalua lazy

        t_block_information* block_to_be_consolidated = (t_block_information*) list_remove(blocks_information,index_of_block_to_consolidate+1);

        consolidate_block_with(master_block,block_to_be_consolidated);
    }

    if (is_valid_index(blocks_information,index_of_block_to_consolidate-1)
        && is_free_block_in_index(blocks_information, index_of_block_to_consolidate-1)){

        t_block_information* block_to_be_consolidated = (t_block_information*) list_remove(blocks_information,index_of_block_to_consolidate-1);

        consolidate_block_with(master_block,block_to_be_consolidated);
    }
}

void free_partition_by_algorithm(t_list* blocks_information){
    t_block_information* block_to_free = dynamic_partition_message_allocator->partition_free_algorithm (blocks_information);
    if (block_to_free != NULL){
        void* position_of_partition_freed = block_to_free->initial_position;
        uint32_t message_id_from_block_to_free = block_to_free->memory_block->message_id;
        uint32_t message_operation_from_block_to_free = block_to_free->memory_block->message_operation;
        empty_block_information(block_to_free);

        //consolido el bloque
        int block_freed_index = block_index_position(block_to_free,blocks_information);
        consolidate_block_for_dynamic_partition(blocks_information,block_freed_index);

        delete_message(message_operation_from_block_to_free,message_id_from_block_to_free,"Victima del algoritmo de reemplazo.");
        log_succesful_free_partition_to_cache(position_of_partition_freed,message_id_from_block_to_free);
    }else{
        log_invalid_free_partition_error();
        free_system();
    }
}

t_block_information* find_block_to_allocate_message(t_list* blocks_information, uint32_t message_size){

    t_block_information* block_information_found = NULL;

    for(uint32_t number_of_partitions_freed = 0; block_information_found == NULL; number_of_partitions_freed++){

        block_information_found = dynamic_partition_message_allocator->available_partition_search_algorithm (message_size, blocks_information, dynamic_partition_message_allocator->min_partition_size);

        if (block_information_found == NULL && number_of_partitions_freed >= dynamic_partition_message_allocator->max_search_tries){
            int amount_of_partitions_before_compaction = list_size(blocks_information);

            dynamic_partition_message_allocator->memory_compaction_algorithm(blocks_information);

            int amount_of_partitions_compacted = amount_of_partitions_before_compaction - list_size(blocks_information);
            number_of_partitions_freed=0;

            log_succesful_memory_compaction(amount_of_partitions_compacted);

            block_information_found = dynamic_partition_message_allocator->available_partition_search_algorithm (message_size, blocks_information, dynamic_partition_message_allocator->min_partition_size);
        }

        if(block_information_found == NULL){
            free_partition_by_algorithm(blocks_information);
        }
    }

    return block_information_found;
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

//ACA ARRANCA LA PAPA
void dynamic_partition_allocate_message(t_identified_message* message,t_list* blocks_information){
    //logica para guardar un mensaje en memoria



    uint32_t message_size = get_size_of(message);

    t_block_information* block_information_found = find_block_to_allocate_message(blocks_information, message_size);

    t_memory_block* memory_block_to_save = build_memory_block_from(message, block_information_found);

    //encontre un block manager disponible, lo spliteo y creo uno nuevo que tenga la memoria restante, que este libre
    t_block_information* new_block_information = save_memory_block_in_block_information(block_information_found,memory_block_to_save);

    // es posible que no haya que crear uno nuevo, si la particion tenia el tamaño exacto necesario
    if (new_block_information != NULL){
        int position = block_index_position(block_information_found,blocks_information);
        list_add_in_index(blocks_information,position+1,(void*) new_block_information);
    }
    log_succesful_save_message_to_cache(message_request_from_identified_message(message),block_information_found->initial_position);

}

t_message_allocator* initialize_dynamic_partition_message_allocator(){

    dynamic_partition_message_allocator = safe_malloc(sizeof(t_message_allocator));
    dynamic_partition_message_allocator->allocate_message_function = dynamic_partition_allocate_message;

    dynamic_partition_message_allocator->min_partition_size = config_get_int_at("TAMANO_MINIMO_PARTICION");
    dynamic_partition_message_allocator->max_search_tries = config_get_int_at("FRECUENCIA_COMPACTACION");
    dynamic_partition_message_allocator->max_partition_size = config_get_int_at("TAMANO_MEMORIA");

    dynamic_partition_message_allocator->available_partition_search_algorithm = get_available_partition_search_algorithm(); //FF/BF
    dynamic_partition_message_allocator->partition_free_algorithm = get_partition_free_algorithm(); //FIFO/LRU
    dynamic_partition_message_allocator->memory_compaction_algorithm = memory_compaction_algorithm;

    return dynamic_partition_message_allocator;
}