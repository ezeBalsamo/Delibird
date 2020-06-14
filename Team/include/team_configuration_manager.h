#ifndef DELIBIRD_TEAM_CONFIGURATION_MANAGER_H
#define DELIBIRD_TEAM_CONFIGURATION_MANAGER_H

void initialize_team_configuration_manager();

char* broker_ip();
char* broker_port();
int time_delay();
int ack_timeout();

#endif //DELIBIRD_TEAM_CONFIGURATION_MANAGER_H
