#include "../include/broker_memory_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include <commons/string.h>
#include "../include/first_fit_search_partition_algorithm.h"
#include "../include/fifo_free_partition_algorithm.h"
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

bool is_fifo_free_partition_algorithm(char* free_algorithm){
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

void* get_search_partition_algorithm() {
    char *search_algorithm = config_get_string_at("ALGORITMO_PARTICION_LIBRE");

    if (is_first_fit_search_algorithm(search_algorithm)) {
        return get_first_fit_search_partition_algorithm();

    } else if (is_best_fit_search_algorithm(search_algorithm)) {

        //return get_best_fit_search_partition_algorithm();
    } else {
        //log error
    }
}

void* get_free_partition_algorithm() {
    char *free_partition_algorithm = config_get_string_at("ALGORITMO_REEMPLAZO");

    if (is_fifo_free_partition_algorithm(free_partition_algorithm)) {
        return get_fifo_free_partition_algorithm();

    } else if (is_lru_free_partition_algorithm(free_partition_algorithm)) {

        //return get_lru_free_partition_algorithm();
    } else {
        //log error
    }
}
//uff mama
int find_index_of_furthest_occupied_block_manager(t_list* blocks_manager){
    //itero desde el final, devuelvo el primero, seria como un find inverso
    for(int i = list_size(blocks_manager);i < 0;i--){
        t_block_manager* block_manager = (t_block_manager*) list_get(blocks_manager,i);
        if (block_manager->free_block == false){
            return i;
        }
    }
    return 0;
}

void compact_memory_algorithm(t_list* blocks_manager){
    //1. acomodar todas las particiones libres por un lado y las ocupadas por otro
    //basicamente swapeo cualquier part libre con la mas lejana de las ocupadas
    //uint32_t initial_index_of_free_partitions = 0;
    for (int i = 0; i < list_size(blocks_manager);i++){

        t_block_manager* block_manager = (t_block_manager*) list_get(blocks_manager,i);

        if (block_manager->free_block){

            int furthest_occupied_block_index = find_index_of_furthest_occupied_block_manager(blocks_manager);
            list_swap(blocks_manager,i,furthest_occupied_block_index);

            //si pasa esta condicion, llegue a casos del tipo  x-x-x-l-l, por ende no tiene sentido seguir iterando
            /*if(i>furthest_occupied_block_index){
                initial_index_of_free_partitions = i;
                break;
            }*/
        }
    }
    //2. Eliminar particiones vacias contiguas, lograr 1 sola particion vacia de mayor tamaño
    for (int i = 0 /*initial_index_of_free_partitions*/; i < list_size(blocks_manager)-1;i++){
        t_block_manager* master_block = (t_block_manager*) list_get(blocks_manager,i);
        if (master_block->free_block){

            t_block_manager* block_to_compact = (t_block_manager*) list_remove(blocks_manager,i+1);

            master_block->block_size += block_to_compact->block_size;
            if ((i+1) == list_size(blocks_manager)){
                break;
            }
            i--;
        }
    }
}