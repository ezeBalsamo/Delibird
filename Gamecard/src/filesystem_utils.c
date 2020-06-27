#include "../include/filesystem_utils.h"
#include "../include/gamecard_configuration_manager.h"
#include <commons/string.h>
#include <string.h>
#include <sys/file.h>

char* create_block_path(char block_pointer[]){
    return string_from_format("%s/Blocks/%s.bin", tallgrass_mount_point(), block_pointer);
}

int split(char* string, int index, char* separator, char found_string[])
{

    int i = 1;
    int initial_size = strlen(string);

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

void set_open(FILE* file_pointer){
    change_file_open_flag(file_pointer, "Y");
}

void set_closed(FILE* file_pointer){
    change_file_open_flag(file_pointer, "N");
}

void close_metadata(char* metadata_path) {

    FILE *file_pointer = fopen(metadata_path, "r+");
    int fd = fileno(file_pointer); //el file descriptor para los flocks :D
    flock(fd, LOCK_SH);

    set_closed(file_pointer);

    fclose(file_pointer);
    flock(fd, LOCK_UN);
}