#ifndef DELIBIRD_BROKER_LOGS_MANAGER_H
#define DELIBIRD_BROKER_LOGS_MANAGER_H

#include "publisher_message_mode.h"
#include "subscriber_context_provider.h"
#include "queue_message_status.h"

void initialize_broker_logs_manager();

void log_succesful_connection_of_a_process();

void log_succesful_subscription_process(t_subscriber_context* subscriber_context);

void log_succesful_new_message_pushed_to_a_queue(uint32_t message_id, uint32_t queue_code);

void log_succesful_message_sent_to_a_suscriber(uint32_t message_id, t_subscriber_context* subscriber_context);

void log_succesful_message_received_by(t_subscriber_context* subscriber_context, uint32_t message_id);

void log_succesful_initialize_queue_context_provider();

void log_successful_initialize_broker_memory_manager();

void log_server_initial_status();

void log_received_message_of(uint32_t operation_code);

void log_succesful_message_sent_to_suscribers(uint32_t message_id);

void log_succesful_get_and_update_subscribers_to_send(uint32_t message_id);

void log_update_of_message_id_received_for(t_subscriber_context* subscriber_context);

void log_succesful_eliminating_message_of_a_queue(uint32_t message_id, char* reason);

void log_invalid_operation_to_message_role_identifier_error(uint32_t operation);

void log_failed_to_receive_ack_error(t_subscriber_context* subscriber_context);

void log_subscriber_disconnection(t_subscriber_context* subscriber_context);

void log_subscriber_not_found_in_message_status_subscribers_error(t_subscriber_context* subscriber_context, uint32_t message_id);

void log_subscriber_not_found_in_queue_subscribers(t_subscriber_context* subscriber_context, uint32_t queue_code);

void log_cache_dump_information(char* cache_information);

void log_message_status_not_found_in_queue_error(uint32_t message_id);

void log_no_subscribers_for_message_with_id(uint32_t message_id);

void free_broker_logs_manager();

void log_invalid_operation_to_save_message_error();

void log_succesful_save_message_to_cache(uint32_t message_id, uint32_t message_size, void* message_position);

void log_succesful_memory_compaction(int amount_of_partitions_compacted);

void log_succesful_free_partition_to_cache(void* message_position, uint32_t message_id);

void log_succesful_memory_compaction_as_buddies(void* master_block_position, void* buddy_block_position);

void log_invalid_minimum_partition_size_for_buddy_system_error();

void log_invalid_free_partition_error();

void log_succesful_memory_compaction_as_buddies(void* master_block_position,void* buddy_block_position);

void log_invalid_memory_size_for_buddy_system_error();

void log_block_information_with_id_not_found(uint32_t message_id);

#endif //DELIBIRD_BROKER_LOGS_MANAGER_H
