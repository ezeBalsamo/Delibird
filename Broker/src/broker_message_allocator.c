#include <broker_logs_manager.h>
#include <commons/collections/dictionary.h>
#include <dynamic_partition_message_allocator.h>
#include <buddy_system_message_allocator.h>
#include <stdlib.h>
#include "broker_message_allocator.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/configuration_manager.h"
#include <deserialization_information_content_provider.h>

t_dictionary* allocation_algorithms;
t_message_allocator* message_allocator;
uint64_t fifo_id;

void initialize_allocation_algorithms(){
    allocation_algorithms = dictionary_create();
    dictionary_put(allocation_algorithms,"PD", (void*) initialize_dynamic_partition_message_allocator);
    dictionary_put(allocation_algorithms, "BS", (void*) initialize_buddy_system_message_allocator);
}

//consolida los bloques en memoria, no en la lista administrativa (asume que conseguiste el bloque con un list_removes)
void consolidate_block_with(t_block_information* master_block,t_block_information* block_to_be_consolidated){
    master_block->block_size += block_to_be_consolidated ->block_size;

    if(master_block->initial_position > block_to_be_consolidated ->initial_position){

        master_block->initial_position = block_to_be_consolidated ->initial_position;
    }

    free(block_to_be_consolidated);
}

uint32_t block_size_for(t_memory_block *  memory_block_to_save){
    bool block_to_save_bigger_than_min_partition_size = memory_block_to_save->message_size > message_allocator->min_partition_size;

    return block_to_save_bigger_than_min_partition_size ? memory_block_to_save->message_size : message_allocator->min_partition_size;
}

int block_index_position(t_block_information* block_to_find,t_list* blocks_information){

    for(int i= 0; i < list_size(blocks_information); i++){
        t_block_information* block_found = (t_block_information*) list_get(blocks_information,i);
        if(block_found->initial_position == block_to_find->initial_position){
            return i;
        }
    }
    free_system();
    return -1;
}

t_memory_block* build_memory_block_from(uint32_t message_id, uint32_t message_size, t_deserialization_information* deserialization_information, t_block_information* block_information) {

    t_memory_block *memory_block_to_save = safe_malloc(sizeof(t_memory_block));

    memory_block_to_save -> message_id = message_id;
    memory_block_to_save -> correlative_message_id = correlative_message_id_using(deserialization_information);
    memory_block_to_save -> message_operation = operation_queue_using(deserialization_information);

    memory_block_to_save -> message_size = message_size;
    memory_block_to_save -> message = block_information -> initial_position;
    store_message_using(&memory_block_to_save -> message, deserialization_information);


    memory_block_to_save -> lru_value = current_time_in_milliseconds();
    memory_block_to_save -> memory_block_id = get_next_fifo_id();

    if(memory_block_to_save -> message_size > message_allocator -> max_partition_size){
        log_invalid_operation_to_save_message_error();
        free_system();
    }

    return memory_block_to_save;
}

uint64_t get_next_fifo_id(){
    //se podria chequear ademas de que no supere la cantidad maxima de un uint64
    fifo_id++;
    return fifo_id;
}

void initialize_message_allocator() {

    char* memory_algorithm = config_get_string_at("ALGORITMO_MEMORIA");
    void* initialize_message_allocator_from_dictionary = dictionary_get(allocation_algorithms, memory_algorithm);
    t_message_allocator *(*initialize_message_allocator_function)() = (t_message_allocator *(*)()) initialize_message_allocator_from_dictionary;
    message_allocator = initialize_message_allocator_function();
    fifo_id = 0;
}

void allocate_with_message_allocator_in_blocks_information(uint32_t message_id, t_deserialization_information* deserialization_information, t_list* blocks_information){

    message_allocator -> allocate_message_function (message_id, deserialization_information, blocks_information);
}

void free_message_allocator(){
    dictionary_destroy(allocation_algorithms);
    free(message_allocator);
}