#ifndef DELIBIRD_FILESYSTEM_UTILS_H
#define DELIBIRD_FILESYSTEM_UTILS_H

#include <stdio.h>

char* create_block_path(char block_pointer[]);
int split(char* string, int index, char* separator, char found_string[]);
void set_open(FILE* file_pointer);
void set_closed(FILE* file_pointer);
void change_file_open_flag(FILE* file_pointer, char* new_flag);
void close_metadata(char* metadata_path);

#endif //DELIBIRD_FILESYSTEM_UTILS_H
