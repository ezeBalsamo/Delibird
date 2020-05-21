#ifndef DELIBIRD_BROKER_LOGS_MANAGER_H
#define DELIBIRD_BROKER_LOGS_MANAGER_H

void initialize_broker_logs_manager();

void log_succesful_connection_of_a_process();

void log_succesful_subscription_process();

void log_succesful_new_message_pushed_to_a_queue();

void log_succesful_send_message_to_a_suscriber();

void log_succesful_start_up();

void log_succesful_initialize_config_manager();

void log_succesful_initialize_queue_message_manager();

void log_succesful_queues_creation();

void log_succesful_subscribers_list_creation();

void log_server_first_status();

void log_structure_recieved();

void log_succesful_subscription();

void log_pushed_message();

void log_published_message();

#endif //DELIBIRD_BROKER_LOGS_MANAGER_H
