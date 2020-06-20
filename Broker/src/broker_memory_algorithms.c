#include "../include/broker_memory_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include <commons/string.h>
#include "../include/first_fit_available_partition_search_algorithm.h"
#include "../include/fifo_partition_free_algorithm.h"
#include "../include/dynamic_partition_message_allocator.h"
#include "../include/broker_memory_algorithms.h"
#include "../../Utils/include/t_list_extension.h"

bool is_dynamic_memory_algorithm(char* memory_algorithm){
    return string_equals_ignore_case("PD",memory_algorithm);
}

bool is_buddy_system_memory_algorithm(char* memory_algorithm){
    return string_equals_ignore_case("BD",memory_algorithm);
}

bool is_first_fit_search_algorithm(char* search_algorithm){
    return string_equals_ignore_case("FF",search_algorithm);
}

bool is_best_fit_search_algorithm(char* search_algorithm){
    return string_equals_ignore_case("BF",search_algorithm);
}

bool is_lru_free_partition_algorithm(char* free_algorithm){
    return string_equals_ignore_case("LRU",free_algorithm);
}

bool is_fifo_partition_free_algorithm(char* free_algorithm){
    return string_equals_ignore_case("FIFO",free_algorithm);
}

t_message_allocator* initialize_message_allocator(){
    char* memory_algorithm = config_get_string_at("ALGORITMO_MEMORIA");

    if(is_dynamic_memory_algorithm(memory_algorithm)){

        return initialize_dynamic_partition_message_allocator();

    }else if (is_buddy_system_memory_algorithm(memory_algorithm)){

        //return initialize_buddy_system_message_allocator();
    }else{
        //log error
    }
}

void* get_available_partition_search_algorithm() {
    char *search_algorithm = config_get_string_at("ALGORITMO_PARTICION_LIBRE");

    if (is_first_fit_search_algorithm(search_algorithm)) {
        return get_first_fit_available_partition_search_algorithm();

    } else if (is_best_fit_search_algorithm(search_algorithm)) {

        //return get_best_fit_search_partition_algorithm();
    } else {
        //log error
    }
}

void* get_free_partition_algorithm() {
    char *free_partition_algorithm = config_get_string_at("ALGORITMO_REEMPLAZO");

    if (is_fifo_partition_free_algorithm(free_partition_algorithm)) {
        return get_fifo_partition_free_algorithm();

    } else if (is_lru_free_partition_algorithm(free_partition_algorithm)) {

        //return get_lru_free_partition_algorithm();
    } else {
        //log error
    }
}
//uff mama
int find_index_of_furthest_occupied_block_information(t_list* blocks_information){
    //itero desde el final, devuelvo el primero, seria como un find inverso
    for(int i = list_size(blocks_information);i < 0;i--){
        t_block_information* block_information = (t_block_information*) list_get(blocks_information,i);
        if (block_information->is_free == false){
            return i;
        }
    }
    return 0;
}

void compact_memory_algorithm(t_list* blocks_information){

    for (int i = 0; i < list_size(blocks_information);i++){

        t_block_information* block_information = (t_block_information*) list_get(blocks_information,i);

        if (block_information->is_free){

            int furthest_occupied_block_index = find_index_of_furthest_occupied_block_information(blocks_information);
            list_swap(blocks_information,i,furthest_occupied_block_index);

        }
    }
    //2. Eliminar particiones vacias contiguas, lograr 1 sola particion vacia de mayor tamaño
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