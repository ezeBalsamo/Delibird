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

char* get_file_name_with_extension(char* path, char* extension) {

        DIR* directory_stream_found;
        struct dirent* file;
        directory_stream_found = opendir(path);
        t_list* names_of_directory_files = list_create();

        if (!directory_stream_found) {
            log_directory_could_not_open_in_path_error();
            free_system();
        }

        while ((file = readdir(directory_stream_found)) != NULL) {
            char* name = file -> d_name;
            unsigned char type = file -> d_type;  //Los archivos son de tipo DT_REG.

            if(type == DT_REG){
                list_add(names_of_directory_files, name);
            }
        }

        bool _is_a_file_with_extension(char* file_name){
            return string_contains(file_name, extension);
        }

        char* file_name_found = list_find(names_of_directory_files, (bool (*) (void*))_is_a_file_with_extension);

        if(file_name_found == NULL){
            log_file_not_found_error(extension);
            free_system();
        }

        list_destroy(names_of_directory_files);

        closedir(directory_stream_found);

        return file_name_found;
}

char* absolute_path_for_config(){

    char* project_path = module_absolute_path();
    char* config_name = get_file_name_with_extension(project_path, ".config");
    char* config_absolute_path = string_from_format("%s/%s", project_path, config_name);

    free(project_path);
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