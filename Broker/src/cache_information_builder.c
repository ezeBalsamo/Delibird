#include "cache_information_builder.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include <commons/temporal.h>
#include <stdio.h>
#include <string.h>
#include <broker_memory_manager.h>
#include <commons/string.h>

char* pointer_address_as_string(void* block_position_pointer){
    char position[32];
    int amount_of_characters = sprintf(position,"%p",block_position_pointer);

    char *initial_address_position = safe_malloc(amount_of_characters);

    memcpy(initial_address_position,&position[0], amount_of_characters);

    return initial_address_position;
}
// 0x000 - 0x3FF [X] Size:1024b LRU:<VALOR> Cola: <COLA> ID: <ID>
// 0x400 - 0x7FE [L] Size:1024b
char* block_info_as_string(t_block_manager* block){
    char* block_info = string_new();

    char* symbol = "[X] ";
    if (block->free_block){
        symbol = "[L] ";
    }
    string_append(&block_info,symbol);
    char* initial_address_position = pointer_address_as_string(block->initial_position);

    char* last_address_position = pointer_address_as_string(block->initial_position+block->block_size);

    char* block_size_info = string_new();
    string_append(&block_size_info, "Size: ");
    string_append(&block_size_info, string_itoa(block->block_size));
    string_append(&block_size_info, "b "); //piden la b de byte

    char* memory_block_info = string_new();
    if (block->free_block == false){
        char* lru_value = string_new();
        string_append(&lru_value,"LRU: ");
        char* time = string_itoa(block->memory_block->lru_value);
        string_append(&lru_value,time);

        char* queue = string_new();
        string_append(&queue," Cola: ");
        string_append(&queue,queue_name_of(block->memory_block->message_operation));

        char* id = string_new();
        string_append(&id," ID: ");
        string_append(&id,string_itoa(block->memory_block->message_id));

        string_append(&memory_block_info,lru_value);
        string_append(&memory_block_info,queue);
        string_append(&memory_block_info,id);
    }

    string_append(&block_info,initial_address_position);
    string_append(&block_info, " - ");
    string_append(&block_info,last_address_position);
    string_append(&block_info,"   ");
    string_append(&block_info,block_size_info);
    string_append(&block_info,memory_block_info);

    return block_info;
}

char* get_cache_info(t_list* blocks_manager){
    char* cache_partitions_info = string_new();

    for(int i = 0; i<list_size(blocks_manager); i++){
        t_block_manager* block = (t_block_manager*) list_get(blocks_manager,i);

        string_append(&cache_partitions_info,"Particion ");
        char* partition_number = string_itoa(i);

        //Particion N:
        string_append(&cache_partitions_info,partition_number);
        string_append(&cache_partitions_info,": ");
        //resto de la info
        char* block_info = block_info_as_string(block);

        string_append(&cache_partitions_info,block_info);
        string_append(&cache_partitions_info,"\n");
    }
    return cache_partitions_info;
}

char* cache_info_builder(t_list* blocks_manager){
    char* cache_info = string_new();
    string_append(&cache_info,"------------------\nDump: ");

    char* actual_time = temporal_get_string_time();
    string_append(&cache_info,actual_time); //TODO: dia actual

    string_append(&cache_info,"\n");
    char* cache_blocks_info = get_cache_info(blocks_manager);
    string_append(&cache_info, cache_blocks_info);

    string_append(&cache_info,"\n------------------");

    return cache_info;

}
