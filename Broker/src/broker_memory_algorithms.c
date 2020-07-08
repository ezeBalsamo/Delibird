#include "../include/broker_memory_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <best_fit_available_partition_search_algorithm.h>
#include <stdlib.h>
#include <lru_partition_free_algorithm.h>
#include "../include/first_fit_available_partition_search_algorithm.h"
#include "../include/fifo_partition_free_algorithm.h"
#include "../include/dynamic_partition_message_allocator.h"
#include "../include/broker_memory_algorithms.h"
#include "../../Utils/include/t_list_extension.h"

t_dictionary* algorithms;

void reposition_free_block_to_end(t_block_information *block_to_reposition, t_list *blocks_information,int block_index){
    void* initial_position_to_occupy_for_next_block = block_to_reposition->initial_position;
    //ajustar las posiciones de todos los bloques (como si se hubiera borrado el que quiero reposicionar)
    for(int i = block_index+1; i<list_size(blocks_information)-1;i++){
        t_block_information* block_to_adjust = list_get(blocks_information,i);
        block_to_adjust ->initial_position = initial_position_to_occupy_for_next_block;

        initial_position_to_occupy_for_next_block = block_to_adjust->initial_position+block_to_adjust->block_size;
    }
    //moverlo al final de la memoria y de la lista administrativa
    list_remove(blocks_information,block_index);
    list_add(blocks_information,block_to_reposition);

    t_block_information* last_block = list_get_last_element(blocks_information);
    block_to_reposition->initial_position = last_block->initial_position + last_block->block_size;
}

void initialize_broker_memory_algorithms(){
    algorithms = dictionary_create();
    dictionary_put(algorithms,"FIFO", (void*)fifo_partition_free_algorithm);
    dictionary_put(algorithms,"LRU", (void*)lru_partition_free_algorithm);
    dictionary_put(algorithms,"FF", (void*)first_fit_available_partition_search_algorithm);
    dictionary_put(algorithms, "BF", (void*)best_fit_available_partition_search_algorithm);
    dictionary_put(algorithms,"PD", (void*)initialize_dynamic_partition_message_allocator);
}

t_message_allocator* initialize_message_allocator() {

    char* memory_algorithm = config_get_string_at("ALGORITMO_MEMORIA");
    void* initialize_message_allocator_from_dictionary = dictionary_get(algorithms, memory_algorithm);
    t_message_allocator *(*initialize_message_allocator_function)() = (t_message_allocator *(*)()) initialize_message_allocator_from_dictionary;

    return initialize_message_allocator_function();
}

void* get_available_partition_search_algorithm() {
    char *search_algorithm = config_get_string_at("ALGORITMO_PARTICION_LIBRE");

    return dictionary_get(algorithms,search_algorithm);
}

void* get_partition_free_algorithm() {
    char *partition_free_algorithm = config_get_string_at("ALGORITMO_REEMPLAZO");

    return dictionary_get(algorithms,partition_free_algorithm);
}

int find_index_of_furthest_occupied_block_information(t_list* blocks_information){
    //itero desde el final, devuelvo el primero, seria como un find inverso
    for(int i = list_size(blocks_information);i > 0;i--){
        t_block_information* block_information = (t_block_information*) list_get(blocks_information,i-1);
        if (!block_information->is_free ){
            return i-1;
        }
    }
    return -1;
}

void combine_all_free_partitions(t_list* blocks_information){
    for (int i = 0; i < list_size(blocks_information)-1;i++){
        t_block_information* master_block = (t_block_information*) list_get(blocks_information,i);
        if (master_block->is_free){

            t_block_information* block_to_compact = (t_block_information*) list_remove(blocks_information,i+1);

            master_block->block_size += block_to_compact->block_size;
            if ((i+1) == list_size(blocks_information)){
                break;
            }
            i--;
        }
    }
}
bool all_blocks_are_free_according_to(int furthest_occupied_block_index){
    return furthest_occupied_block_index == -1;
}

void memory_compaction_algorithm(t_list* blocks_information){

    for (int i = 0; i < list_size(blocks_information);i++){

        t_block_information* block_information = (t_block_information*) list_get(blocks_information,i);

        if (block_information->is_free){
            reposition_free_block_to_end(block_information,blocks_information,i);
        }
    }
    //Combinar particiones vacias contiguas a 1 sola particion vacia de mayor tamaño
    combine_all_free_partitions(blocks_information);
}

bool can_save_message(t_block_information* block_information, uint32_t message_size, uint32_t min_partition_size){
    bool enough_size_for_message = ((t_block_information*) block_information)->block_size >= message_size;
    bool block_is_free = ((t_block_information*) block_information)->is_free;
    bool block_is_usable = ((t_block_information*) block_information)->block_size >= min_partition_size;
    return enough_size_for_message && block_is_free && block_is_usable;
}

void empty_block_information(t_block_information* block_found){
    block_found->is_free = true;
    free(block_found->memory_block);
    block_found->memory_block = NULL;
}

void consolidate_block_with(t_block_information* master_block,t_block_information* block_to_be_consolidated){
    master_block->block_size += block_to_be_consolidated ->block_size;

    if(master_block->initial_position > block_to_be_consolidated ->initial_position){

        master_block->initial_position = block_to_be_consolidated ->initial_position;
    }

    free(block_to_be_consolidated);
}

bool is_free_block_in_index(t_list* blocks_information, int index){
    return ((t_block_information*) list_get(blocks_information,index))->is_free;
}

void consolidate_block(t_list* blocks_information,int index_of_block_to_consolidate){
    t_block_information* master_block = (t_block_information*) list_get(blocks_information,index_of_block_to_consolidate);

    if (is_valid_index(blocks_information,index_of_block_to_consolidate+1)){
        if(is_free_block_in_index(blocks_information,index_of_block_to_consolidate+1)){ //en caso de que no sea lazy evaluation

            t_block_information* block_to_be_consolidated = (t_block_information*) list_remove(blocks_information,index_of_block_to_consolidate+1);

            consolidate_block_with(master_block,block_to_be_consolidated);
        }
    }

    if (is_valid_index(blocks_information,index_of_block_to_consolidate-1)){
        if(is_free_block_in_index(blocks_information, index_of_block_to_consolidate-1)){
            t_block_information* block_to_be_consolidated = (t_block_information*) list_remove(blocks_information,index_of_block_to_consolidate-1);

            consolidate_block_with(master_block,block_to_be_consolidated);
        }
    }
}