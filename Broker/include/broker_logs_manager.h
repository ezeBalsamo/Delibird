#ifndef DELIBIRD_BROKER_LOGS_MANAGER_H
#define DELIBIRD_BROKER_LOGS_MANAGER_H

#include "publisher_message_mode.h"
#include "subscriber_context_provider.h"

void initialize_broker_logs_manager();

void log_succesful_connection_of_a_process();

void log_succesful_subscription_process(t_subscriber_context* subscriber_context);

void log_succesful_new_message_pushed_to_a_queue(t_identified_message* identified_message, uint32_t queue_code);

void log_succesful_message_sent_to_a_suscriber(t_request* request, t_subscriber_context* subscriber_context);

void log_succesful_initialize_queue_context_provider();

void log_server_initial_status();

void log_structure_received(void* serialized_request);

void log_succesful_message_sent_to_suscribers(t_request* request);

void log_succesful_get_and_update_subscribers_to_send(t_identified_message* identified_message);

void log_no_subscribers_for_request(t_request* request);

void log_succesful_all_messages_of_a_queue_sent_to(char* process_id);

void log_invalid_operation_to_message_role_identifier_error(uint32_t operation);

void log_failed_to_receive_ack_error(t_subscriber_context* subscriber_context);

void log_subscriber_disconnection(t_subscriber_context* subscriber_context);

void log_received_unknown_operation_error();

void log_subscriber_not_found_in_message_status_subscribers_error(t_subscriber_context* subscriber_context, t_identified_message* identified_message);

void log_subscriber_not_found_in_queue_subscribers_error(t_subscriber_context* subscriber_context, uint32_t queue_code);

void free_broker_logs_manager();

#endif //DELIBIRD_BROKER_LOGS_MANAGER_H
