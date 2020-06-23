#include "cache_information_builder.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include <commons/temporal.h>
#include <broker_memory_manager.h>
#include <commons/string.h>
#include <stdlib.h>

char* pointer_address_as_string(void* block_position_pointer){

    char* address_position = string_from_format("%p",block_position_pointer);

    return address_position;
}

char* memory_block_as_string(t_memory_block* memory_block){

    char* lru_value = string_new();
    string_append(&lru_value,"LRU: ");
    char* time = string_itoa(memory_block->lru_value);
    string_append(&lru_value,time);

    char* queue = string_new();
    string_append(&queue," Cola: ");
    string_append(&queue,queue_name_of(memory_block->message_operation));

    char* id = string_new();
    string_append(&id," ID: ");
    string_append(&id,string_itoa(memory_block->message_id));

    return string_from_format("%s%s%s",lru_value,queue,id);
}

char* block_information_availabity_as_string(t_block_information* block_information){
    return block_information -> is_free ? "[L]" : "[X]";
}
// 0x000 - 0x3FF [X] Size:1024b LRU:<VALOR> Cola: <COLA> ID: <ID>
// 0x400 - 0x7FE [L] Size:1024b
char* block_information_as_string(t_block_information* block_information){

    char* symbol = block_information_availabity_as_string(block_information);

    char* initial_address_position = pointer_address_as_string(block_information->initial_position);

    char* last_address_position = pointer_address_as_string(block_information->initial_position+block_information->block_size);

    char* block_size = string_itoa(block_information->block_size);

    char* block_size_info = string_from_format("Size: %sb ",block_size);



    char* memory_block_info = string_new();
    if (!block_information->is_free){

        char* memory_block = memory_block_as_string(block_information->memory_block);
        string_append(&memory_block_info,memory_block);
    }

    char* block_info = string_from_format("%s - %s %s %s%s",initial_address_position,last_address_position,symbol,block_size_info,memory_block_info);

    return block_info;
}

char* build_using(t_list* blocks_information){
    char* cache_partitions_info = string_new();

    for(int i = 0; i<list_size(blocks_information); i++){

        t_block_information* block = (t_block_information*) list_get(blocks_information,i);

        char* partition_number = string_itoa(i);

        char* block_info = block_information_as_string(block);

        char* partition_info = string_from_format("Partición %s: %s\n",partition_number,block_info);

        string_append(&cache_partitions_info,partition_info);

        free(partition_number);
        free(block_info);
        free(partition_info);
    }

    return cache_partitions_info;
}

char* cache_information_builder(t_list* blocks_information){
    char* actual_time = temporal_get_string_time();

    char* cache_blocks_info = build_using(blocks_information);

    char* cache_info = string_from_format("------------------\nDump: %s\n%s\n------------------",actual_time,cache_blocks_info);
    free(actual_time);
    free(cache_blocks_info);
    return cache_info;
}
