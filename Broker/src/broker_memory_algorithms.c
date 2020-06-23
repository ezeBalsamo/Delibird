#include "../include/broker_memory_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include "../include/first_fit_available_partition_search_algorithm.h"
#include "../include/fifo_partition_free_algorithm.h"
#include "../include/dynamic_partition_message_allocator.h"
#include "../include/broker_memory_algorithms.h"
#include "../../Utils/include/t_list_extension.h"

t_dictionary* algorithms;

void initialize_broker_memory_algorithms(){
    algorithms = dictionary_create();
    dictionary_put(algorithms,"FIFO", (void*)fifo_partition_free_algorithm);
    dictionary_put(algorithms,"FF", (void*)first_fit_available_partition_search_algorithm);
}

bool is_dynamic_memory_algorithm(char* memory_algorithm){
    return string_equals_ignore_case("PD",memory_algorithm);
}

bool is_buddy_system_memory_algorithm(char* memory_algorithm){
    return string_equals_ignore_case("BD",memory_algorithm);
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

    return dictionary_get(algorithms,search_algorithm);
}

void* get_free_partition_algorithm() {
    char *free_partition_algorithm = config_get_string_at("ALGORITMO_REEMPLAZO");

    return dictionary_get(algorithms,free_partition_algorithm);
}

int find_index_of_furthest_occupied_block_information(t_list* blocks_information){
    //itero desde el final, devuelvo el primero, seria como un find inverso
    for(int i = list_size(blocks_information);i > 0;i--){
        t_block_information* block_information = (t_block_information*) list_get(blocks_information,i-1);
        if (block_information->is_free == false){
            return i-1;
        }
    }
    return 0;
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

void memory_compaction_algorithm(t_list* blocks_information){

    for (int i = 0; i < list_size(blocks_information);i++){

        t_block_information* block_information = (t_block_information*) list_get(blocks_information,i);

        if (block_information->is_free){

            int furthest_occupied_block_index = find_index_of_furthest_occupied_block_information(blocks_information);
            //solo swapear si el bloque vacio necesita ser reordenado
            if(i < furthest_occupied_block_index){
                list_swap(blocks_information,i,furthest_occupied_block_index);
            }


        }
    }
    //Combinar particiones vacias contiguas a 1 sola particion vacia de mayor tamaño
    combine_all_free_partitions(blocks_information);
}