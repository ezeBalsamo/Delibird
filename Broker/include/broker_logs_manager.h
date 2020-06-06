#ifndef DELIBIRD_BROKER_LOGS_MANAGER_H
#define DELIBIRD_BROKER_LOGS_MANAGER_H

#include "publish_message_mode.h"

void initialize_broker_logs_manager();

void log_succesful_connection_of_a_process();

void log_succesful_subscription_process(int subscriber);

void log_succesful_new_message_pushed_to_a_queue(t_identified_message* identified_message);

void log_succesful_message_sent_to_a_suscriber(void* message_sent);

void log_succesful_initialize_queue_message_manager();

void log_succesful_initialize_subscriber_manager();

void log_server_initial_status();

void log_structure_received(void* serialized_request);

void log_succesful_message_sent_to_suscribers(t_identified_message* identified_message);

void log_succesful_get_and_update_subscribers_to_send();

void log_no_subscribers_for_request(t_identified_message* identified_message);

void log_succesful_all_messages_of_a_queue_sent_to(int subscriber);

void log_invalid_operation_to_message_role_identifier_error(uint32_t operation);

void log_ack_received_error();

void log_received_unknown_operation_error();

void free_broker_logs_manager();

#endif //DELIBIRD_BROKER_LOGS_MANAGER_H
