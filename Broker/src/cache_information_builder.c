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
char* block_info_as_string(t_block_manager* block){
    char* block_info = string_new();
    char* free_block = "[X]";
    if (block->free_block){
        free_block = "[L]";
    }

    char* initial_address_position = pointer_address_as_string(block->initial_position);

    char* last_address_position = pointer_address_as_string(block->initial_position+block->block_size);

    char* block_size_info = "Size: ";
    string_append(&block_size_info, string_itoa(block->block_size));
    string_append(&block_size_info, "b "); //piden la b de byte

    char* memory_block_info = string_new();
    if (block->memory_block != NULL){
        char* lru_value = "LRU: ";
        string_append(&lru_value,string_itoa(block->memory_block->lru_value));

        char* queue = "Cola: ";
        string_append(&queue,queue_name_of(block->memory_block->message_queue_code));

        char* id = "ID: ";
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
    char* cache_partitions_info = "";

    for(int i = 0; list_size(blocks_manager); i++){
        t_block_manager* block = (t_block_manager*) list_get(blocks_manager,i);

        char* partition_desc = "Particion ";
        char* partition_number = string_itoa(i);

        //Particion N:
        string_append(&partition_desc,partition_number);
        string_append(&partition_desc,": ");
        //resto de la info
        char* block_info = block_info_as_string(block);

        string_append(&cache_partitions_info,partition_desc);
        string_append(&cache_partitions_info,block_info);
        string_append(&cache_partitions_info,"\n");
    }
    return cache_partitions_info;
}

char* cache_info_builder(t_list* blocks_manager){
    char* cache_info = "------------------\nDump: ";

    string_append(&cache_info,temporal_get_string_time()); //TODO: dia actual

    string_append(&cache_info, get_cache_info(blocks_manager));

    string_append(&cache_info,"\n------------------");

    return cache_info;

}
