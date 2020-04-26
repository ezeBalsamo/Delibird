#ifndef DELIBIRD_CONFIGURATION_MANAGER_H
#define DELIBIRD_CONFIGURATION_MANAGER_H

void initialize_configuration_manager_named(char* config_name);

int config_get_int_at(char*);
char* config_get_string_at(char*);

#endif //DELIBIRD_CONFIGURATION_MANAGER_H
