#include "cache_information_builder.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include <commons/temporal.h>
#include <stdio.h>
#include <string.h>
#include <broker_memory_manager.h>
#include <commons/string.h>

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

// 0x000 - 0x3FF [X] Size:1024b LRU:<VALOR> Cola: <COLA> ID: <ID>
// 0x400 - 0x7FE [L] Size:1024b
char* block_info_as_string(t_block_information* block){

    char* symbol = "[X] ";
    if (block->is_free){
        symbol = "[L] ";
    }

    char* initial_address_position = pointer_address_as_string(block->initial_position);

    char* last_address_position = pointer_address_as_string(block->initial_position+block->block_size);

    char* block_size = string_itoa(block->block_size);

    char* block_size_info = string_from_format("Size: %sb ",block_size);



    char* memory_block_info = string_new();
    if (block->is_free == false){
        char* memory_block = memory_block_as_string(block->memory_block);
        string_append(&memory_block_info,memory_block);
    }

    char* block_info = string_from_format("%s - %s %s %s%s%s",initial_address_position,last_address_position,symbol,block_size_info,memory_block_info);

    return block_info;
}

char* build_using(t_list* blocks_information){
    char* cache_partitions_info = string_new();

    for(int i = 0; i<list_size(blocks_information); i++){

        t_block_information* block = (t_block_information*) list_get(blocks_information,i);

        char* partition_number = string_itoa(i);
        char* block_info = block_info_as_string(block);

        char* partition_info = string_from_format("Partición %s: %s\n",partition_number,block_info);

        string_append(&cache_partitions_info,partition_info);
    }
    return cache_partitions_info;
}

char* cache_information_builder(t_list* blocks_information){
    char* cache_info = string_new();
    string_append(&cache_info,"------------------\nDump: ");

    char* actual_time = temporal_get_string_time();
    string_append(&cache_info,actual_time); //TODO: dia actual

    string_append(&cache_info,"\n");
    char* cache_blocks_info = build_using(blocks_information);
    string_append(&cache_info, cache_blocks_info);

    string_append(&cache_info,"\n------------------");

    return cache_info;

}
