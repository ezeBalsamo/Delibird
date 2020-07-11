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
        t_list* directory_files = list_create();

        if (!directory_stream_found) {
            log_directory_could_not_open_in_path_error();
            free_system();
        }

        while ((file = readdir(directory_stream_found)) != NULL) {
            char* name = file -> d_name;
            unsigned char type = file -> d_type;  //Los archivos son de tipo 8. Las carpetas de tipo 4.

            if(type == 8){
                list_add(directory_files, file);
            }
        }

        bool _is_a_file_with_extension(struct dirent* file){
            return string_contains(file -> d_name, extension);
        }

        struct dirent* file_found = list_find(directory_files, (bool (*) (void*))_is_a_file_with_extension);
        list_destroy(directory_files);

        closedir(directory_stream_found);

        return file_found -> d_name;
}

char* absolute_path_for_config(){

    char* project_path = module_absolute_path();
    char* config_name = get_file_name_with_extension(project_path, "config");
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