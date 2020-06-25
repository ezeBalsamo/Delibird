#ifndef DELIBIRD_FILESYSTEM_H
#define DELIBIRD_FILESYSTEM_H

#include <stdint.h>
#include <commons/collections/list.h>

#define FILESYSTEM_METADATA 1
#define ARCHIVO_METADATA 2
#define BLOQUE 3

#define MAXLINE 150

enum File_Type {
    FS_METADATA, FILE_METADATA,
    BLOCK
};

typedef struct File_Information{
    uint32_t file_type;
    void* (*reader_function) (char* file_path);
}t_file_information;

typedef struct {
    int positionX;
    int positionY;
    int pokemonQuantity;
}pokemon_block_line;

typedef struct {
    int posX;
    int posY;
}position;

typedef struct {
    char* directory; //'Y' or 'N'
    int size;
    char* blocks;
    char* open; //'Y' or 'N'
}file_metadata;

typedef struct {
    int Block_size;
    int Blocks;
    char* Magic_Number;//TALL_GRASS
}fs_metadata;

void initialize_filesystem();
void* read_file_of_type(int file_type, char* file_name);
t_list* create_positions_list(t_list *data_list);
uint32_t amount_of_positions(t_list* positions_list);
void close_metadata(char* metadata_path);

#endif //DELIBIRD_FILESYSTEM_H
