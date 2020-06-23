#ifndef DELIBIRD_FILESYSTEM_H
#define DELIBIRD_FILESYSTEM_H

#define FS_METADATA 1
#define FILE_METADATA 2
#define BLOCK 3

#define MAXLINE 150

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
    char directory[1]; //'Y' or 'N'
    int size;
    char blocks[4092];
    char open[1]; //'Y' or 'N'
}file_metadata;

typedef struct {
    int Block_size;
    int Blocks;
    char Magic_Number[11];//TALL_GRASS
}fs_metadata;

void initialize_filesystem();
void* read_file_of_type(int file_type, char* file_name);

#endif //DELIBIRD_FILESYSTEM_H
