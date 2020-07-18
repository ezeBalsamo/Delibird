#include <commons/collections/list.h>
#include <broker_memory_manager.h>
#include <stdlib.h>
#include "memory_compaction_algorithm.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/configuration_manager.h"

bool is_free(t_block_information* block_information){
    return block_information -> is_free;
}

int next_occupied_block_information_index_using(int block_information_index, t_list* blocks_information){

    if(block_information_index + 1 == list_size(blocks_information)){
        return block_information_index;
    }

    int next_occupied_block_index = block_information_index + 1;
    t_block_information* block_information = list_get(blocks_information, next_occupied_block_index);

    while(block_information -> is_free){
        next_occupied_block_index++;

        if(next_occupied_block_index == list_size(blocks_information)){
            return block_information_index;
        }

        block_information = list_get(blocks_information, next_occupied_block_index);
    }

    return next_occupied_block_index;
}

void* reposition_block_information(void* initial_position, t_block_information* block_information){
    block_information -> initial_position = initial_position;
    return block_information -> initial_position + block_information -> block_size;
}

void find_and_load_first_free_block_information(t_list* blocks_information,
                                                int* first_free_block_information_index,
                                                t_block_information** first_free_block_information){

    bool found = false;

    for(int i = 0; !found; i++){

        t_block_information* block_information = list_get(blocks_information, i);

        if(is_free(block_information)){
            *first_free_block_information_index = i;
            *first_free_block_information = block_information;
            found = true;
        }
    }
}

void reposition_occupied_blocks_information(int first_free_block_information_index,
                                            void** next_initial_position,
                                            int* occupied_blocks_size,
                                            t_list* blocks_information){

    int block_index = first_free_block_information_index;

    while(block_index < list_size(blocks_information)){

        int next_occupied_block_information_index =
                next_occupied_block_information_index_using(block_index, blocks_information);

        if(block_index != next_occupied_block_information_index){

            t_block_information* occupied_block_information =
                    list_get(blocks_information, next_occupied_block_information_index);

            *next_initial_position =
                    reposition_block_information(*next_initial_position, occupied_block_information);

            *occupied_blocks_size += occupied_block_information -> block_size;
            block_index = next_occupied_block_information_index;
        }else{
            block_index = list_size(blocks_information);
        }
    }
}

void build_and_add_free_partition_block_information(void* next_initial_position,
                                                    int occupied_blocks_size,
                                                    t_list* blocks_information){

    int total_memory_size = config_get_int_at("TAMANO_MEMORIA");

    t_block_information* free_block_information = safe_malloc(sizeof(t_block_information));
    free_block_information -> is_free = true;
    free_block_information -> initial_position = next_initial_position;
    free_block_information -> memory_block = NULL;
    free_block_information -> block_size = total_memory_size - occupied_blocks_size;

    list_add(blocks_information, free_block_information);
}

void memory_compaction_algorithm(t_list* blocks_information) {

    int occupied_blocks_size = 0;

    int first_free_block_information_index;
    t_block_information* first_free_block_information;

    find_and_load_first_free_block_information(blocks_information,
                                               &first_free_block_information_index,
                                               &first_free_block_information);

    void* next_initial_position = first_free_block_information -> initial_position;

    reposition_occupied_blocks_information(first_free_block_information_index,
                                           &next_initial_position,
                                           &occupied_blocks_size,
                                           blocks_information);

    list_remove_and_destroy_all_by_condition(blocks_information, (bool (*)(void *)) is_free, free);

    build_and_add_free_partition_block_information(next_initial_position, occupied_blocks_size, blocks_information);
}