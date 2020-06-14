#include "../include/broker_memory_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include <commons/string.h>
#include <dynamic_partition_memory_manager.h>
#include "../include/broker_memory_algorithms.h"
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

void* get_free_partition_algorithm() {
    char *free_partition_algorithm = config_get_string_at("ALGORITMO_REEMPLAZO");

    if (is_lru_free_partition_algorithm(free_partition_algorithm)) {
        return get_fifo_free_partition_algorithm();

    } else if (is_fifo_free_partition_algorithm(free_partition_algorithm)) {

        //return get_best_fit_search_partition_algorithm();
    } else {
        //log error
    }
}

void compact_memory_algorithm(){};