#include <commons/string.h>
#include <stdlib.h>
#include <unistd.h>
#include "paths.h"
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <commons/collections/list.h>
#include <garbage_collector.h>
#include <general_logs.h>

char* get_file_name_from_path(char* path){

    char* logs_path_reversed = string_reverse(path);
    char** logs_path_splitted = string_split(logs_path_reversed, "/");
    char* file_name = string_reverse(logs_path_splitted[0]);

    free(logs_path_reversed);
    string_iterate_lines(logs_path_splitted, (void (*)(char *)) free);
    free(logs_path_splitted);
    return file_name;
}

char* module_absolute_path(){
    char* executable_path = getcwd(NULL, 0);
    char* reversed_path_for_split = string_reverse(executable_path);
    char** reversed_path_splitted = string_n_split(reversed_path_for_split, 2, "/");
    char* project_absolute_path = string_reverse(reversed_path_splitted[1]);

    free(executable_path);
    free(reversed_path_for_split);
    string_iterate_lines(reversed_path_splitted, (void (*)(char *)) free);
    free(reversed_path_splitted);

    return project_absolute_path;
}

t_list* regular_files_names_in_directory(DIR* directory, char* path){

    struct dirent* file;
    t_list* regular_files_names = list_create();

    if (!directory) {
        log_directory_could_not_open_in_path_error(path);
        free_system();
    }

    while ((file = readdir(directory)) != NULL) {
        char* name = file -> d_name;
        unsigned char type = file -> d_type;  //Los archivos regulares son de tipo DT_REG.

        if(type == DT_REG){
            list_add(regular_files_names, name);
        }
    }

    return regular_files_names;
}

char* find_file_name_with_extension(t_list* files_names, char* extension){

    bool _is_a_file_name_with_extension(char* file_name){
        return string_contains(file_name, extension);
    }

    char* file_name_found = list_find(files_names, (bool (*) (void*))_is_a_file_name_with_extension);

    if(file_name_found == NULL){
        log_file_not_found_error(extension);
        free_system();
    }

    return file_name_found;

}

char* get_file_name_with_extension(DIR* directory, char* path, char* extension) {

    t_list* regular_files_names_found = regular_files_names_in_directory(directory, path);
    char* file_found = find_file_name_with_extension(regular_files_names_found, extension);

    list_destroy(regular_files_names_found);

    return file_found;
}

char* absolute_path_for_config(){

    char* project_path = module_absolute_path();
    DIR* directory = opendir(project_path);

    char* config_name = get_file_name_with_extension(directory, project_path, ".config");
    char* config_absolute_path = string_from_format("%s/%s", project_path, config_name);

    free(project_path);
    closedir(directory);
    return config_absolute_path;
}

char* logs_path(){

    char* project_path = module_absolute_path();
    char* logs_path = string_from_format("%s/logs", project_path);

    free(project_path);
    return logs_path;
}

char* absolute_path_for_log_named(char* log_name){
    char* path = logs_path();
    char* log_absolute_path = string_from_format("%s/%s", path, log_name);

    free(path);
    return log_absolute_path;
}

bool exists_file_at(char* path){
    struct stat stat_buffer;
    return stat(path, &stat_buffer) != -1;
}