#include "../include/configuration_manager.h"
#include "commons/string.h"
#include "commons/config.h"

t_config* config;

void initialize_configuration_manager_named(char* config_name){

    char* path = string_from_format("../%s.config", config_name);
    config = config_create(path);
}

int config_get_int_at(char* key){
    return config_get_int_value(config, key);
}

char* config_get_string_at(char* key){
    return config_get_string_value(config, key);
}