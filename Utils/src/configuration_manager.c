#include "../include/configuration_manager.h"
#include "commons/config.h"
#include <roots.h>
#include <stdlib.h>
#include <general_logs.h>

t_config* config;

void initialize_configuration_manager_named(char* config_name){

    char* config_absolute_path = absolute_path_for_config_named(config_name);
    config = config_create(config_absolute_path);
    free(config_absolute_path);

    log_succesful_initialize_config_manager();
}

int config_get_int_at(char* key){
    return config_get_int_value(config, key);
}

char* config_get_string_at(char* key){
    return config_get_string_value(config, key);
}

char** config_get_char_array_at(char* key){
    return config_get_array_value(config, key);
}

void free_configuration_manager(){
    config_destroy(config);
}