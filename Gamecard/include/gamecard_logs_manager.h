#ifndef DELIBIRD_GAMECARD_LOGS_MANAGER_H
#define DELIBIRD_GAMECARD_LOGS_MANAGER_H

#include "file_system.h"
#include <stdint.h>

void initialize_gamecard_logs_manager();
void log_failed_attempt_to_communicate_with_broker_from_gamecard(char* default_action);
void log_initiating_communication_retry_process_with_broker_from_gamecard();
void log_succesful_retry_of_communication_with_broker_from_gamecard();
void log_failed_retry_of_communication_with_broker_from_gamecard();
void log_thread_sleep_time_configuration_error_from_gamecard();
void log_queue_thread_create_error_from_gamecard();
void log_query_performer_not_found_error_from_gamecard_for(uint32_t operation);
void log_file_system_metadata_info(t_file_system_metadata* pointer_file_system_metadata);
void log_file_metadata_info(t_file_metadata* pointer_file_metadata);
void log_block_metadata_info(int32_t x, int32_t y, int32_t quantity);
void log_unknown_file_type_error();
void log_pokemon_not_found_error(t_catch_pokemon* pokemon_to_subtract);
void log_pokemon_file_destroyed(char* pokemon_name);
void free_gamecard_logs_manager();

#endif //DELIBIRD_GAMECARD_LOGS_MANAGER_H
