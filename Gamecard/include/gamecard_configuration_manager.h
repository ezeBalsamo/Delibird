#ifndef DELIBIRD_GAMECARD_CONFIGURATION_MANAGER_H
#define DELIBIRD_GAMECARD_CONFIGURATION_MANAGER_H

#include <stdint.h>

void initialize_gamecard_configuration_manager();
char* broker_ip();
char* broker_port();
uint32_t ack_timeout();
uint32_t operation_delay_time_getter();
uint32_t operation_retry_time_getter();
char* tallgrass_mount_point();

#endif //DELIBIRD_GAMECARD_CONFIGURATION_MANAGER_H
