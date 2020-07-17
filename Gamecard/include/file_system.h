#ifndef DELIBIRD_FILE_SYSTEM_H
#define DELIBIRD_FILE_SYSTEM_H

#include "../../Utils/include/common_structures.h"
#include <stdint.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h>

enum File_Types {
    FILE_SYSTEM_METADATA, FILE_METADATA,
    BLOCK
};

typedef struct Pokemon_block_line{
    int32_t position_x;
    int32_t position_y;
    uint32_t quantity;
}t_pokemon_block_line;

typedef struct File_metadata{
    char* directory; //'Y' or 'N'
    uint32_t size;
    char* blocks;
    char* open; //'Y' or 'N'
}t_file_metadata;

typedef struct File_system_metadata{
    uint32_t block_size;
    uint32_t blocks;
    char* magic_number;//TALL_GRASS
}t_file_system_metadata;

void initialize_file_system();
void* read_file_of_type(uint32_t file_type, char* file_name);
t_list* create_positions_list(t_list *data_list);
uint32_t amount_of_positions(t_list* positions_list);
t_file_system_metadata* read_file_system_metadata(char* file_path);
t_file_metadata* read_file_metadata(char* file_path);
t_list* read_block(char* file_path);
void write_pokemon_metadata(t_file_metadata* metadata_file_information, char* pokemon_metadata_path);
void write_pokemon_blocks(t_list* pokemon_data_list, t_file_metadata* metadata_file_information);
bool subtract_or_remove_from(t_list* blocks_information, t_catch_pokemon* pokemon_to_subtract);
void add_or_modify_to(t_list* blocks_information, t_new_pokemon* pokemon_to_add);
t_list* data_to_write(t_new_pokemon* new_pokemon);
void create_pokemon_metadata(char* pokemon_name);
char* get_new_block();
t_bitarray* bitmap_get();

#endif //DELIBIRD_FILE_SYSTEM_H
