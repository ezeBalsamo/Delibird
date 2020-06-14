#include "../include/broker_memory_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include <commons/string.h>
#include <dynamic_partition_memory_manager.h>

t_memory_manager* memory_manager;

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

void* get_allocate_message_algorithm(){
    char* memory_algorithm = config_get_string_at("ALGORITMO_MEMORIA");

    if(is_dynamic_memory_algorithm(memory_algorithm)){

        return get_dynamic_partition_allocate_message_algorithm();

    }else if (is_buddy_system_memory_algorithm(memory_algorithm)){

        //return get_buddy_system_allocate_message_algorithm();
    }else{
        //log error
    }
}

t_block_manager* first_fit_search_partition_algorithm(){
    return NULL;
}

void* get_first_fit_search_partition_algorithm(){
    return first_fit_search_partition_algorithm;
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
//---
void fifo_partition_algorithm(){}

void* get_fifo_free_partition_algorithm(){
    return fifo_partition_algorithm;
}

void* get_free_partition_algorrithm() {
    char *free_partition_algorithm = config_get_string_at("ALGORITMO_REEMPLAZO");

    if (is_lru_free_partition_algorithm(free_partition_algorithm)) {
        return get_fifo_free_partition_algorithm();

    } else if (is_fifo_free_partition_algorithm(free_partition_algorithm)) {

        //return get_best_fit_search_partition_algorithm();
    } else {
        //log error
    }
}
//---
void initialize_broker_memory_manager(){

    memory_manager = safe_malloc(sizeof(t_memory_manager));
    memory_manager->blocks_manager = list_create();

    memory_manager->allocate_message_function = get_allocate_message_algorithm(); //CASTEAR ESTO?
    memory_manager->find_available_partition_algorithm = get_search_partition_algorithm();
    memory_manager->free_partition_algorithm = get_free_partition_algorrithm();

    memory_manager->min_partition_size = config_get_int_at("TAMANO_MINIMO_PARTICION");
    memory_manager->max_search_tries = config_get_int_at("FRECUENCIA_COMPACTACION");

    uint32_t memory_size = config_get_int_at("TAMANO_MEMORIA");
    //initial block manager, with all the memory
    t_block_manager* initial_block_manager = safe_malloc(sizeof(t_block_manager));
    initial_block_manager->free_block = true;
    initial_block_manager->block_size = memory_size;
    initial_block_manager->initial_position = safe_malloc(memory_size);
    initial_block_manager->memory_block = NULL;

    list_add(memory_manager->blocks_manager,(void*) initial_block_manager);
}

void allocate_message(t_identified_message* message){
    memory_manager->allocate_message_function (message);
}