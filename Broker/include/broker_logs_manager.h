#ifndef DELIBIRD_BROKER_LOGS_MANAGER_H
#define DELIBIRD_BROKER_LOGS_MANAGER_H

void initialize_broker_logs_manager();

void log_succesful_connection_of_a_process();

void log_succesful_subscription_process();

void log_succesful_new_message_pushed_to_a_queue(void* serialized_request);

void log_succesful_message_sent_to_a_suscriber(void* message_sent);

void log_succesful_initialize_queue_message_manager();

void log_succesful_queues_creation();

void log_succesful_subscribers_list_creation();

void log_server_initial_status();

void log_structure_received(void* serialized_request);

void log_succesful_message_sent_to_suscribers(void* serialized_request);

void log_no_subscribers_for_request(void* serialized_request);

void received_unknown_operation_error();

void free_broker_logs_manager();

#endif //DELIBIRD_BROKER_LOGS_MANAGER_H
