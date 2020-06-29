#ifndef DELIBIRD_FILE_SYSTEM_H
#define DELIBIRD_FILE_SYSTEM_H

#include <stdint.h>
#include <commons/collections/list.h>

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

#endif //DELIBIRD_FILE_SYSTEM_H
