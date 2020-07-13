#include <broker_memory_manager.h>
#include <broker_memory_algorithms.h>
#include <broker_logs_manager.h>
#include <broker_message_allocator.h>
#include <memory_compaction_algorithm.h>
#include "buddy_system_message_allocator.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/t_list_extension.h"
#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>

t_message_allocator *message_allocator;

unsigned long get_pointer_position_as_decimal(void* pointer){
    char* pointer_as_string = string_from_format("%p",pointer);
    int amount_of_hexadecimal_prefix_characters = 2;
    char* pointer_as_hexadecimal = string_substring_from(pointer_as_string,amount_of_hexadecimal_prefix_characters+1); //Ignore 0x from hexadecimal position
    int hexadecimal_base = 16;
    return strtoul(pointer_as_hexadecimal, NULL, hexadecimal_base);
}

//CONDICION BUDDIES: PosA == DirB XOR TamA   && PosB == DirA XOR TamB (PPTS NATASHA)
bool blocks_are_buddies(t_block_information* block_information_A, t_block_information* block_information_B){
    uint32_t block_size_A = block_information_A->block_size;
    uint32_t block_size_B = block_information_B->block_size;

    unsigned long position_of_A = get_pointer_position_as_decimal(block_information_A->initial_position);
    unsigned long position_of_B = get_pointer_position_as_decimal(block_information_B->initial_position);

    unsigned long position_B_xor_size_A = position_of_B ^ block_size_A;
    unsigned long position_A_xor_size_B = position_of_A ^ block_size_B;

    bool same_size = block_size_A == block_size_B;
    bool position_A_equals_position_B_xor_size_A = position_of_A == position_B_xor_size_A;
    bool position_B_equals_position_A_xor_size_B = position_of_B == position_A_xor_size_B;

    return same_size && position_A_equals_position_B_xor_size_A && position_B_equals_position_A_xor_size_B;
}

bool is_valid_block_for_buddy_compaction(t_list* blocks_information,t_block_information* master_block, int index_of_candidate){
   bool valid_index = is_valid_index(blocks_information,index_of_candidate);
    bool block_is_free = false;
    bool block_is_buddy = false;

   if (valid_index)
       block_is_free = is_free_block_in_index(blocks_information,index_of_candidate);

   if (valid_index && block_is_free){
       t_block_information* block_candidate_for_buddy_compaction = (t_block_information*) list_get(blocks_information, index_of_candidate);
       block_is_buddy = blocks_are_buddies(master_block,block_candidate_for_buddy_compaction);
   }
   return valid_index && block_is_free && block_is_buddy;
}
//unify block with buddies if possible
void associate_with_buddies(t_list* blocks_information,t_block_information* master_block){
    bool left_is_buddy = true;
    bool right_is_buddy = true;

    while(left_is_buddy || right_is_buddy){

        int master_block_current_index = block_index_position(master_block,blocks_information);

        if (is_valid_block_for_buddy_compaction(blocks_information,master_block,master_block_current_index-1)){
            void* master_block_position = master_block->initial_position;
            t_block_information* buddy_block = (t_block_information*) list_remove(blocks_information, master_block_current_index-1);
            consolidate_block_with(master_block,buddy_block);
            right_is_buddy = true;
            log_succesful_memory_compaction_as_buddies(master_block_position, master_block_position);
            }else{
            left_is_buddy = false;
        }

        if (is_valid_block_for_buddy_compaction(blocks_information,master_block,master_block_current_index+1)){
            t_block_information* buddy_block = (t_block_information*) list_remove(blocks_information, master_block_current_index+1);
            consolidate_block_with(master_block,buddy_block);
            left_is_buddy = true;
        }else{
            right_is_buddy = false;
        }
    }

}

//split block to buddies if possible, recursively
void disassociate_buddy_if_possible(t_list* blocks_information, t_block_information* block_information_found, uint32_t message_size){

    while(block_information_found->block_size/2 >= message_size){
        uint32_t new_block_size =  block_information_found->block_size / 2;

        block_information_found->block_size = new_block_size;

        t_block_information* new_buddy_block_information = safe_malloc(sizeof(t_block_information));
        new_buddy_block_information->is_free = true;
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
        uint32_t message_id_from_block_to_free = block_to_free->memory_block->message_id;

        empty_block_information(block_to_free);
        log_succesful_free_partition_to_cache(position_of_partition_freed,message_id_from_block_to_free);

        //consolido el bloque con buddy system
        associate_with_buddies(blocks_information,block_to_free);


    }else{
        log_invalid_free_partition_error();
        free_system();
    }
}

t_block_information* find_block_to_allocate_message_for_buddy(t_list* blocks_information, uint32_t message_size){
    for ever{

        t_block_information* block_information_found = message_allocator->available_partition_search_algorithm (message_size, blocks_information, message_allocator->min_partition_size);
        if (block_information_found != NULL){
            //if buddy found is too big it may need to be split
            disassociate_buddy_if_possible(blocks_information, block_information_found, message_size);
            return block_information_found;
        }

        free_partition_by_algorithm_for_buddy(blocks_information);
    }

}

//LA PAPA DEL BUDDY SYSTEM
void partition_allocate_message(t_identified_message* message,t_list* blocks_information){
    //logica para guardar un mensaje en memoria

    uint32_t message_size = get_size_of(message);

    t_block_information* block_information_found = find_block_to_allocate_message_for_buddy(blocks_information, message_size);

    t_memory_block* memory_block_to_save = build_memory_block_from(message, block_information_found);

    block_information_found->is_free = false;
    block_information_found->memory_block = memory_block_to_save;

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