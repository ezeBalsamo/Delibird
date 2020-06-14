#ifndef DELIBIRD_CONFIGURATION_MANAGER_H
#define DELIBIRD_CONFIGURATION_MANAGER_H

#include <commons/collections/list.h>

void initialize_configuration_manager_named(char* config_name);

int config_get_int_at(char* key);
char* config_get_string_at(char* key);
char** config_get_char_array_at(char* key);
t_list* all_config_values();

void free_configuration_manager();
#endif //DELIBIRD_CONFIGURATION_MANAGER_H
