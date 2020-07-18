#include <open_files_structure.h>
#include "file_system_utils.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "stdbool.h"
#include <stdlib.h>
#include <commons/string.h>
#include <commons/process.h>

pthread_mutex_t open_files_mutex;
t_list* open_files_list;

void free_structure_and_close_file(char* metadata_path){
	close_metadata(metadata_path);
	free(metadata_path);
}

void add_to_open_files(char* metadata_path){
	pthread_mutex_lock(&open_files_mutex);
	list_add(open_files_list, metadata_path);
	pthread_mutex_unlock(&open_files_mutex);
}

void remove_from_open_files(char* metadata_path){

	bool _are_equals_metadata_path(char* path_open){
		return string_equals_ignore_case(metadata_path, path_open);
	}

	pthread_mutex_lock(&open_files_mutex);
	list_remove_by_condition(open_files_list, (void*) _are_equals_metadata_path);
	pthread_mutex_unlock(&open_files_mutex);
}

void close_open_files(){

    list_destroy_and_destroy_elements(open_files_list, (void (*)(void *)) free_structure_and_close_file);
}

void initialize_open_files_list(){
	open_files_list = list_create();
	safe_mutex_initialize(&open_files_mutex);
}