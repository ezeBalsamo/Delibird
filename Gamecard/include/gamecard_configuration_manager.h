#ifndef DELIBIRD_GAMECARD_CONFIGURATION_MANAGER_H
#define DELIBIRD_GAMECARD_CONFIGURATION_MANAGER_H

void initialize_gamecard_configuration_manager();
char* broker_ip();
char* broker_port();
int ack_timeout();
int op_delay_time();
int op_retry_time();
char* tallgrass_mount_point();

#endif //DELIBIRD_GAMECARD_CONFIGURATION_MANAGER_H
