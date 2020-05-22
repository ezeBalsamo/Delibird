#include <commons/string.h>
#include <stdlib.h>
#include <unistd.h>
#include "roots.h"

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

char* absolute_path_for_config_named(char* config_name){

    char* project_path = module_absolute_path();
    char* config_absolute_path = string_from_format("%s/%s.config", project_path, config_name);

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