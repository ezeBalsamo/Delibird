#include "../include/file_system_utils.h"
#include "../include/gamecard_configuration_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include <commons/string.h>
#include <commons/config.h>
#include <string.h>
#include <stdlib.h>
#include <sys/file.h>
#include <unistd.h>

void list_destroy_and_free_elements(t_list* list){
	if(list_is_empty(list)) {
        list_destroy(list);
	}
	else{
	    list_destroy_and_destroy_elements(list,free);
	}
}

char* get_pokemon_name_from_path(char* pokemon_path){

    char** path_array = string_split(pokemon_path, "/");

    uint32_t path_array_length = sizeof(path_array);

    char* pokemon_name = string_duplicate(path_array[path_array_length]);
    free_char_array(path_array);

    return pokemon_name;
}

char* block_line_to_string(t_pokemon_block_line *line){
    return string_from_format("%d-%d=%d\n",line -> position_x, line -> position_y, line -> quantity);
}

char* create_block_path(char block_pointer[]){
    return string_from_format("%s/Blocks/%s.bin", tallgrass_mount_point(), block_pointer);
}

uint32_t split(char* string, uint32_t index, char* separator, char found_string[])
{

    uint32_t i = 1;
    uint32_t initial_size = strlen(string);

    char string_aux[initial_size];
    strcpy(string_aux, string);

    char delimiter[5];
    strcpy(delimiter, separator);

    char* pointer = strtok(string_aux, delimiter);

    while (pointer != NULL)
    {
        if (i == index)
        {
            strcpy(found_string, pointer);
        }
        i++;
        pointer = strtok(NULL, delimiter);
    }

    return i-1;
}

void change_file_open_flag(FILE* file_pointer, char* new_flag){

    fseek(file_pointer, -2, SEEK_END); // 1 para la y, otro por el \n

    char var[1];
    fscanf(file_pointer, "%c", var);

    if(var[0] == '='){
        fseek(file_pointer, -1, SEEK_END);
    }
    if(var[0] == 'N' || var[0] == 'Y'){
        fseek(file_pointer, -2, SEEK_END);
    }

    fprintf(file_pointer, "%s", new_flag);
}

bool is_open(t_file_metadata* file_metadata){
    return string_equals_ignore_case(file_metadata -> open, "Y");
}

void set_open(FILE* file_pointer){
    change_file_open_flag(file_pointer, "Y");
}

void set_closed(FILE* file_pointer){
    change_file_open_flag(file_pointer, "N");
}

void close_metadata(char* metadata_path) {

    FILE *file_pointer = fopen(metadata_path, "r+");
    uint32_t file_descriptor = fileno(file_pointer); //el file descriptor para los flocks :D
    flock(file_descriptor, LOCK_SH);

    set_closed(file_pointer);

    fclose(file_pointer);
    flock(file_descriptor, LOCK_UN);
}

t_file_system_metadata* read_file_system_metadata_from_config(t_config* metadata_config){

    t_file_system_metadata* file_system_metadata = safe_malloc(sizeof(t_file_system_metadata));

    file_system_metadata -> block_size = config_get_int_value(metadata_config, "BLOCK_SIZE");
    file_system_metadata -> blocks = config_get_int_value(metadata_config, "BLOCKS");
    file_system_metadata -> magic_number = string_duplicate(config_get_string_value(metadata_config, "MAGIC_NUMBER"));

    return file_system_metadata;
}

t_file_metadata* read_file_metadata_from_config(t_config* metadata_config){

    t_file_metadata* file_metadata = safe_malloc(sizeof(t_file_metadata));

    file_metadata -> directory = string_duplicate(config_get_string_value(metadata_config, "DIRECTORY"));
    file_metadata -> size = config_get_int_value(metadata_config, "SIZE");
    file_metadata -> blocks = string_duplicate(config_get_string_value(metadata_config, "BLOCKS"));
    file_metadata -> open = string_duplicate(config_get_string_value(metadata_config, "OPEN"));

    return file_metadata;
}

void unlock_file_during_time(uint32_t file_descriptor, uint32_t time_in_seconds){
    flock(file_descriptor,LOCK_UN);
    sleep(time_in_seconds);
    flock(file_descriptor,LOCK_SH);
}

void free_file_system_metadata(t_file_system_metadata* file_system_metadata){
    free(file_system_metadata -> magic_number);
    free(file_system_metadata);
}

void free_metadata_file(t_file_metadata* file_metadata_path){
    free(file_metadata_path -> directory);
    free(file_metadata_path -> blocks);
    free(file_metadata_path -> open);
    free(file_metadata_path);
}