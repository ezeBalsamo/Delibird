#ifndef DELIBIRD_TEAM_LOGS_MANAGER_H
#define DELIBIRD_TEAM_LOGS_MANAGER_H

#include <stdint.h>

void initialize_team_logs_manager();
void log_failed_attempt_to_communicate_with_broker();
void log_initiating_communication_retry_process_with_broker();
void log_failed_retry_of_communication_with_broker();
void log_succesful_retry_of_communication_with_broker();
void log_thread_sleep_time_configuration_error();
void log_queue_thread_create_error();
void log_no_locations_found_for(char* pokemon_name);
void log_query_performer_not_found_error_for(uint32_t operation);
void log_trainer_thread_create_error();
void log_succesful_creation_of_thread_of_trainer(uint32_t sequential_number);
void log_zero_schedulable_threads_error();
void log_synchronizable_trainer_not_found_error_for(uint32_t sequential_number);

void free_team_logs_manager();

#endif //DELIBIRD_TEAM_LOGS_MANAGER_H
