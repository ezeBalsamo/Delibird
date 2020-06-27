#include "../include/filesystem_files_information.h"
#include "../include/filesystem.h"
#include "../../Utils/include/common_structures.h"
#include <stdlib.h>

t_list* all_files_information;

t_file_information* file_information_with_code(uint32_t file_type_code){

    bool _is_for(void* file_information){
        return ((t_file_information*) file_information) -> file_type == file_type_code;
    }
    t_file_information* file_information_found = list_find(all_files_information, _is_for);

    if(!file_information_found){
        log_unknown_file_type_error();
        free_system();
    }

    return file_information_found;
}

void initialize_and_load_fs_metadata_information(){
    t_file_information* file_information = safe_malloc(sizeof(t_file_information));
    file_information -> file_type = FS_METADATA;
    file_information -> reader_function = read_fs_metadata;

    list_add(all_files_information, (void*) file_information);
}

void initialize_and_load_file_metadata_information(){
    t_file_information* file_information = safe_malloc(sizeof(t_file_information));
    file_information -> file_type = FILE_METADATA;
    file_information -> reader_function = read_file_metadata;

    list_add(all_files_information, (void*) file_information);
}

void initialize_and_load_block_information(){
    t_file_information* file_information = safe_malloc(sizeof(t_file_information));
    file_information -> file_type = BLOCK;
    file_information -> reader_function = read_block;

    list_add(all_files_information, (void*) file_information);
}

void initialize_files_information(){

    all_files_information = list_create();

    //inicializo estructuras de informacion de archivos
    initialize_and_load_fs_metadata_information();
    initialize_and_load_file_metadata_information();
    initialize_and_load_block_information();
}

void free_files_information(){
    list_destroy_and_destroy_elements(all_files_information, free);
}