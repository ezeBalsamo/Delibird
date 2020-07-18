#ifndef DELIBIRD_FILE_SYSTEM_UTILS_H
#define DELIBIRD_FILE_SYSTEM_UTILS_H

#include <commons/config.h>
#include <stdio.h>
#include <stdint.h>
#include "file_system.h"
#include <commons/bitarray.h>

char* block_line_to_string(t_pokemon_block_line* line);
char* create_block_path(char block_pointer[]);
uint32_t split(char* string, uint32_t index, char* separator, char found_string[]);
bool is_open(t_file_metadata* file_metadata);
void set_open(FILE* file_pointer);
void set_closed(FILE* file_pointer);
void change_file_open_flag(FILE* file_pointer, char* new_flag);
void close_metadata(char* metadata_path);
t_file_system_metadata* read_file_system_metadata_from_config(t_config* metadata_config);
t_file_metadata* read_file_metadata_from_config(t_config* metadata_config);
void unlock_file_during_time(uint32_t file_descriptor, uint32_t time_in_seconds);
char* get_pokemon_name_from_path(char* pokemon_path);
void free_metadata_file(t_file_metadata* file_metadata_path);
void free_file_system_metadata(t_file_system_metadata* file_system_metadata);


#endif //DELIBIRD_FILE_SYSTEM_UTILS_H
