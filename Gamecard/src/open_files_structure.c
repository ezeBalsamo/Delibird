#include <open_files_structure.h>
#include "file_system_utils.h"
#include "../../Utils/include/pthread_wrapper.h"
#include "stdbool.h"
#include <stdlib.h>
#include <commons/string.h>
#include <commons/process.h>


pthread_mutex_t open_files_mutex;
t_list* open_files_list;

void free_structure_and_close_file(open_file_by_tid* file_to_close_with_tid){
	close_metadata(file_to_close_with_tid -> metadata_path);
	free(file_to_close_with_tid -> metadata_path);
	free(file_to_close_with_tid);
}

open_file_by_tid* create_file_with_tid(char* metadata_path){
	open_file_by_tid* object = safe_malloc(sizeof(open_file_by_tid));
	object -> metadata_path = metadata_path;
	object -> tid = process_get_thread_id();
	return object;
}

void add_to_open_files(char* metadata_path){
	pthread_mutex_lock(&open_files_mutex);
	list_add(open_files_list, create_file_with_tid(metadata_path));
	pthread_mutex_unlock(&open_files_mutex);
}

void remove_from_open_files(char* metadata_path){
	bool _are_equals(open_file_by_tid* object){
		return string_equals_ignore_case(object -> metadata_path, metadata_path);
	}

	pthread_mutex_lock(&open_files_mutex);
	list_remove_by_condition(open_files_list, (void*) _are_equals);
	pthread_mutex_unlock(&open_files_mutex);

}

void close_open_files(){
	unsigned int tid = process_get_thread_id();

	bool  _same_tid(open_file_by_tid* element_of_list){
		return element_of_list -> tid == tid;
	}

	t_list* files_filtered_by_tid = list_filter(open_files_list,(void*)_same_tid);
	list_destroy_and_destroy_elements(files_filtered_by_tid, (void (*)(void *)) free_structure_and_close_file);
}

void initialize_open_files_list(){
	open_files_list = list_create();
}
