#ifndef DELIBIRD_GAMECARD_LOGS_MANAGER_H
#define DELIBIRD_GAMECARD_LOGS_MANAGER_H

#include "filesystem.h"
#include <stdint.h>

void initialize_gamecard_logs_manager();
void log_failed_attempt_to_communicate_with_broker_from_gamecard(char* default_action);
void log_initiating_communication_retry_process_with_broker_from_gamecard();
void log_succesful_retry_of_communication_with_broker_from_gamecard();
void log_failed_retry_of_communication_with_broker_from_gamecard();
void log_thread_sleep_time_configuration_error_from_gamecard();
void log_queue_thread_create_error_from_gamecard();
void log_invalid_operation_to_query_performer_from_gamecard(uint32_t operation);
void log_query_performer_not_found_error_from_gamecard_for(uint32_t operation);
void log_filesystem_metadata_info(fs_metadata* pointer_fs_metadata);
void log_file_metadata_info(file_metadata* pointer_file_metadata);
void log_block_metadata_info(int x, int y, int quantity);
void free_gamecard_logs_manager();

#endif //DELIBIRD_GAMECARD_LOGS_MANAGER_H
