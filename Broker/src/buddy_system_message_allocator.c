#include <broker_memory_manager.h>
#include <broker_memory_algorithms.h>
#include <broker_logs_manager.h>
#include <broker_message_allocator.h>
#include "buddy_system_message_allocator.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/garbage_collector.h"

t_message_allocator *message_allocator;

//split block to buddies if possible, recursively
void disassociate_buddy_if_possible(t_list* blocks_information, t_block_information* block_information_found, uint32_t message_size){

    while(block_information_found->block_size/2 >= message_size){
        uint32_t new_block_size =  block_information_found->block_size / 2;

        block_information_found->block_size = new_block_size;

        t_block_information* new_buddy_block_information = safe_malloc(sizeof(t_block_information));
        new_buddy_block_information->block_size = new_block_size;
        new_buddy_block_information->initial_position = block_information_found->initial_position + new_block_size;

        int block_position = block_index_position(block_information_found,blocks_information);
        list_add_in_index(blocks_information,block_position+1,(void*) new_buddy_block_information);
    }
}

//associates buddies recursively
void free_partition_by_algorithm_for_buddy(t_list* blocks_information){
    t_block_information* block_to_free = message_allocator->partition_free_algorithm (blocks_information);
    if (block_to_free != NULL){
        void* position_of_partition_freed = block_to_free->initial_position;
        empty_block_information(block_to_free);

        //consolido el bloque
        int block_freed_index = block_index_position(block_to_free,blocks_information);
        consolidate_block(blocks_information,block_freed_index);

        log_succesful_free_partition_to_cache(position_of_partition_freed);
    }else{
        log_invalid_free_partition_error();
        free_system();
    }
}

t_block_information* find_block_to_allocate_message_for_buddy(t_list* blocks_information, t_memory_block* memory_block_to_save){
    for ever{

        t_block_information* block_information_found = message_allocator->available_partition_search_algorithm (memory_block_to_save->message_size, blocks_information, message_allocator->min_partition_size);
        if (block_information_found != NULL){
            //if buddy chosen is too big it may need to be split
            disassociate_buddy_if_possible(blocks_information, block_information_found, memory_block_to_save->message_size);
            return block_information_found;
        }

        free_partition_by_algorithm_for_buddy(blocks_information);
    }

}

t_block_information* save_memory_block_in_block_information_for_buddy(t_block_information* block_information_found, t_memory_block* memory_block_to_save){
    //disassociate buddies
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

//LA PAPA DEL BUDDY SYSTEM
void partition_allocate_message(t_identified_message* message,t_list* blocks_information){
    //logica para guardar un mensaje en memoria

    t_memory_block* memory_block_to_save = build_memory_block_from_message(message);

    t_block_information* block_information_found = find_block_to_allocate_message_for_buddy(blocks_information, memory_block_to_save);

    t_block_information* new_block_information = save_memory_block_in_block_information_for_buddy(block_information_found,memory_block_to_save);

    if (new_block_information != NULL){
        int position = block_index_position(block_information_found,blocks_information);
        list_add_in_index(blocks_information,position+1,(void*) new_block_information);
    }
    log_succesful_save_message_to_cache(message_request_from_identified_message(message),block_information_found->initial_position);
}

bool is_power_of_two(int size){
    if (size == 0)
        return false;
    while (size != 1){
        if (size % 2 != 0)
            return false;
        size = size/2;
    }
    return true;
}

bool is_valid_partition_size_for_buddy_system(int partition_size){
    return is_power_of_two(partition_size) && partition_size > 0;
}

int get_valid_minimum_partition_size(){
    int min_partition_size = config_get_int_at("TAMANO_MINIMO_PARTICION");

    if (!is_valid_partition_size_for_buddy_system(min_partition_size)){
        log_invalid_minimum_partition_size_for_buddy_system_error();
        free_system();
    }
    return min_partition_size;
}

int get_valid_total_memory_size(){
    int memory_size = config_get_int_at("TAMANO_MEMORIA");

    if (!is_valid_partition_size_for_buddy_system(memory_size)){
        log_invalid_memory_size_for_buddy_system_error();
        free_system();
    }
    return memory_size;
}

t_message_allocator* initialize_buddy_system_message_allocator(){

    message_allocator = safe_malloc(sizeof(t_message_allocator));
    message_allocator->allocate_message_function = partition_allocate_message;

    message_allocator->min_partition_size = get_valid_minimum_partition_size();
    message_allocator->max_search_tries = config_get_int_at("FRECUENCIA_COMPACTACION");
    message_allocator->max_partition_size = get_valid_total_memory_size();
    message_allocator->available_partition_search_algorithm = get_available_partition_search_algorithm(); //FF/BF
    message_allocator->partition_free_algorithm = get_partition_free_algorithm(); //FIFO/LRU
    message_allocator->memory_compaction_algorithm = memory_compaction_algorithm;

    return message_allocator;
}