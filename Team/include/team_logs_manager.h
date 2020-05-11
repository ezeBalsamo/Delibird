#ifndef DELIBIRD_TEAM_LOGS_MANAGER_H
#define DELIBIRD_TEAM_LOGS_MANAGER_H

void initialize_team_logs_manager();

void log_succesful_start_up();
void log_failed_attempt_to_communicate_with_broker();
void log_initiating_communication_retry_process_with_broker();
void log_failed_retry_of_communication_with_broker();
void log_succesful_retry_of_communication_with_broker();

void log_thread_sleep_time_configuration_error();

#endif //DELIBIRD_TEAM_LOGS_MANAGER_H
