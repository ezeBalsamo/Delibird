#ifndef DELIBIRD_BROKER_LOGS_MANAGER_H
#define DELIBIRD_BROKER_LOGS_MANAGER_H

#include "publisher_message_mode.h"
#include "subscriber_context_provider.h"
#include "queue_message_status.h"

void initialize_broker_logs_manager();

void log_succesful_connection_of_a_process();

void log_succesful_subscription_process(t_subscriber_context* subscriber_context);

void log_succesful_new_message_pushed_to_a_queue(t_identified_message* identified_message, uint32_t queue_code);

void log_succesful_message_sent_to_a_suscriber(t_request* request, t_subscriber_context* subscriber_context);

void log_succesful_initialize_queue_context_provider();

void log_successful_initialize_broker_memory_manager();

void log_server_initial_status();

void log_structure_received(void* serialized_request);

void log_succesful_message_sent_to_suscribers(t_request* request);

void log_succesful_get_and_update_subscribers_to_send(t_identified_message* identified_message);

void log_no_subscribers_for_request(t_request* request);

void log_succesful_all_messages_of_a_queue_sent_to(t_subscriber_context* subscriber_context);

void log_update_of_message_id_received_for(t_subscriber_context* subscriber_context);

void log_succesful_eliminating_message_of_a_queue(t_message_status* message_status);

void log_invalid_operation_to_message_role_identifier_error(uint32_t operation);

void log_failed_to_receive_ack_error(t_subscriber_context* subscriber_context);

void log_subscriber_disconnection(t_subscriber_context* subscriber_context);

void log_subscriber_not_found_in_message_status_subscribers_error(t_subscriber_context* subscriber_context, t_identified_message* identified_message);

void log_subscriber_not_found_in_queue_subscribers(t_subscriber_context* subscriber_context, uint32_t queue_code);

void log_cache_dump_information(char* cache_information);

void log_message_status_not_found_in_queue_error();

void free_broker_logs_manager();

void log_invalid_operation_to_save_message_error();

void log_succesful_save_message_to_cache(t_request* request, void* message_position);

void log_succesful_memory_compaction(int amount_of_partitions_compacted);

void log_succesful_free_partition_to_cache(void* message_position);

#endif //DELIBIRD_BROKER_LOGS_MANAGER_H
