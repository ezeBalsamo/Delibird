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
#include <deserialization_information_content_provider.h>

t_message_allocator* buddy_system_message_allocator;
extern void* initial_position;
void* middle_memory_position;
void* last_memory_position;

unsigned long long get_pointer_position_as_decimal(void* pointer){
    char* pointer_as_string = string_from_format("%p",pointer);
    int amount_of_hexadecimal_prefix_characters = 2;
    char* pointer_as_hexadecimal = string_substring_from(pointer_as_string,amount_of_hexadecimal_prefix_characters); //Ignore 0x123 from hexadecimal position
    int hexadecimal_base = 16;
    unsigned long long pointer_position_as_decimal =  strtoull(pointer_as_hexadecimal, NULL, hexadecimal_base);
    free(pointer_as_string);
    free(pointer_as_hexadecimal);
    return pointer_position_as_decimal;
}

uint32_t decimal_position_in_memory(void* pointer){
    return get_pointer_position_as_decimal(pointer) - get_pointer_position_as_decimal(initial_position);
}

bool have_the_same_size(t_block_information* block_information, t_block_information* another_block_information){
    return block_information -> block_size == another_block_information -> block_size;
}

bool is_a_free_block(t_block_information* block_information){
    return block_information -> is_free;
}

void update_and_consolidate_blocks(t_list* blocks_information, t_block_information* master_block, int candidate_index) {

    void* master_block_position = master_block->initial_position;
    t_block_information* buddy_block = (t_block_information*) list_remove(blocks_information, candidate_index);
    void* buddy_block_position = buddy_block->initial_position;
    consolidate_block_with(master_block,buddy_block);
    log_succesful_memory_compaction_as_buddies(master_block_position, buddy_block_position);
}

//unify block with buddies if possible
void associate_with_buddies(t_list* blocks_information,t_block_information* master_block){
    bool left_is_buddy = true;
    bool right_is_buddy = true;

    while(left_is_buddy || right_is_buddy){
        int master_block_current_index = block_index_position(master_block,blocks_information);

        if(master_block -> initial_position + master_block -> block_size == middle_memory_position){
            right_is_buddy = true;
            update_and_consolidate_blocks(blocks_information, master_block, master_block_current_index + 1);
            continue;
        }

        if(master_block ->initial_position + master_block ->block_size == last_memory_position){
            left_is_buddy = true;
            update_and_consolidate_blocks(blocks_information, master_block, master_block_current_index - 1);
            continue;
        }

        if(master_block_current_index == 0 || master_block -> initial_position == middle_memory_position){
            left_is_buddy = false;
        } else {
            t_block_information* previous_block = list_get(blocks_information, master_block_current_index - 1);
            if(have_the_same_size(master_block,previous_block) && previous_block -> initial_position == initial_position && is_a_free_block(previous_block)){

                left_is_buddy = true;
                update_and_consolidate_blocks(blocks_information, master_block, master_block_current_index - 1);
                continue;
            } else {
                left_is_buddy = false;
            }

            if(master_block_current_index >= 2){
                t_block_information* previous_from_previous_block = list_get(blocks_information, master_block_current_index - 2);
                if(have_the_same_size(previous_block, previous_from_previous_block) && previous_from_previous_block -> initial_position == initial_position){
                    left_is_buddy = false;
                } else {
                    if(have_the_same_size(master_block, previous_block) && is_a_free_block(previous_block)){
                        left_is_buddy = true;
                        update_and_consolidate_blocks(blocks_information, master_block, master_block_current_index - 1);
                        continue;
                    }
                }
            }
        }

        if(master_block_current_index == (list_size(blocks_information) - 1) || (master_block -> initial_position + master_block -> block_size) == middle_memory_position) {
            right_is_buddy = false;
        } else {
            t_block_information* next_block = list_get(blocks_information, master_block_current_index + 1);
            if(have_the_same_size(master_block,next_block) && next_block -> initial_position + next_block -> block_size == last_memory_position && is_a_free_block(next_block)){
                right_is_buddy = true;
                update_and_consolidate_blocks(blocks_information, master_block, master_block_current_index - 1);
                continue;
            } else {
                right_is_buddy = false;
            }

            if(master_block_current_index + 2 < list_size(blocks_information)){
                t_block_information* next_from_next_block = list_get(blocks_information, master_block_current_index + 2);
                if(have_the_same_size(next_block, next_from_next_block) && (next_from_next_block -> initial_position + next_from_next_block -> block_size) == last_memory_position){
                    right_is_buddy = false;
                } else {
                    if(have_the_same_size(master_block, next_block) && is_a_free_block(next_block)){
                        right_is_buddy = true;
                        update_and_consolidate_blocks(blocks_information, master_block, master_block_current_index + 1);
                    }
                }
            }
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
    t_block_information* block_to_free = buddy_system_message_allocator->partition_free_algorithm (blocks_information);

    if (block_to_free != NULL){
        void* position_of_partition_freed = block_to_free->initial_position;
        uint32_t message_id_from_block_to_free = block_to_free->memory_block->message_id;
        uint32_t message_operation_from_block_to_free = block_to_free->memory_block->message_operation;

        empty_block_information(block_to_free);
        log_succesful_free_partition_to_cache(position_of_partition_freed,message_id_from_block_to_free);
        delete_message(message_operation_from_block_to_free,message_id_from_block_to_free,"Victima del algoritmo de reemplazo.");

        //consolido el bloque con buddy system
        associate_with_buddies(blocks_information,block_to_free);
    }else{
        log_invalid_free_partition_error();
        free_system();
    }
}

t_block_information* find_block_to_allocate_message_for_buddy(t_list* blocks_information, uint32_t message_size){
    for ever{

        t_block_information* block_information_found = buddy_system_message_allocator->available_partition_search_algorithm (message_size, blocks_information, buddy_system_message_allocator->min_partition_size);
        if (block_information_found != NULL){
            //if buddy found is too big it may need to be split
            disassociate_buddy_if_possible(blocks_information, block_information_found, message_size);
            return block_information_found;
        }

        free_partition_by_algorithm_for_buddy(blocks_information);
    }

}

//LA PAPA DEL BUDDY SYSTEM
void partition_allocate_message(uint32_t message_id, t_deserialization_information* deserialization_information, t_list* blocks_information){
    //logica para guardar un mensaje en memoria

    uint32_t message_size = message_size_using(deserialization_information);

    t_block_information* block_information_found = find_block_to_allocate_message_for_buddy(blocks_information, message_size);

    t_memory_block* memory_block_to_save = build_memory_block_from(message_id, message_size, deserialization_information, block_information_found);

    block_information_found->is_free = false;
    block_information_found->memory_block = memory_block_to_save;

    log_succesful_save_message_to_cache(message_id, message_size, block_information_found -> initial_position);
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

    buddy_system_message_allocator = safe_malloc(sizeof(t_message_allocator));
    buddy_system_message_allocator->allocate_message_function = partition_allocate_message;

    buddy_system_message_allocator->min_partition_size = get_valid_minimum_partition_size();
    buddy_system_message_allocator->max_search_tries = config_get_int_at("FRECUENCIA_COMPACTACION");
    buddy_system_message_allocator->max_partition_size = get_valid_total_memory_size();
    buddy_system_message_allocator->available_partition_search_algorithm = get_available_partition_search_algorithm(); //FF/BF
    buddy_system_message_allocator->partition_free_algorithm = get_partition_free_algorithm(); //FIFO/LRU
    buddy_system_message_allocator->memory_compaction_algorithm = memory_compaction_algorithm;

    middle_memory_position = initial_position + (buddy_system_message_allocator -> max_partition_size / 2);
    last_memory_position = initial_position + buddy_system_message_allocator -> max_partition_size;

    return buddy_system_message_allocator;
}