#include "../include/file_system_utils.h"
#include "../include/gamecard_configuration_manager.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/configuration_manager.h"
#include <commons/string.h>
#include <commons/config.h>
#include <string.h>
#include <sys/file.h>
#include <unistd.h>

char* block_line_to_string(t_pokemon_block_line *line){
    return string_from_format("%d-%d=%d",line -> pos_x, line -> pos_y, line -> quantity);
    //hay que probar si funca
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

    file_system_metadata -> block_size = config_get_int_at("BLOCK_SIZE");
    file_system_metadata -> blocks = config_get_int_at("BLOCKS");
    file_system_metadata -> magic_number = config_get_string_at("MAGIC_NUMBER");

    return file_system_metadata;
}

t_file_metadata* read_file_metadata_from_config(t_config* metadata_config){

    t_file_metadata* file_metadata = safe_malloc(sizeof(t_file_metadata));

    file_metadata -> directory = config_get_string_value(metadata_config, "DIRECTORY");
    file_metadata -> size = config_get_int_value(metadata_config, "SIZE");
    file_metadata -> blocks = config_get_string_value(metadata_config, "BLOCKS");
    file_metadata -> open = config_get_string_value(metadata_config, "OPEN");

    return file_metadata;
}

void unlock_file_during_time(uint32_t file_descriptor, uint32_t time_in_seconds){
    flock(file_descriptor,LOCK_UN);
    sleep(time_in_seconds);
    flock(file_descriptor,LOCK_SH);
}
