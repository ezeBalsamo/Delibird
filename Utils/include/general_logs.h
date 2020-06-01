#ifndef DELIBIRD_GENERAL_LOGS_H
#define DELIBIRD_GENERAL_LOGS_H

void log_succesful_start_up();
void log_successful_execution();
void log_successful_clean_up();
void log_succesful_initialize_config_manager();
void log_succesful_initialize_pretty_printer();
void log_succesful_initialize_serializable_objects();
void log_get_ifaddrs_error(char* message);
void log_get_socket_fd_error(char* message);
void log_allow_port_reusability(char* message);
void log_bind_error(char* message);
void log_listen_error(char* message);
void log_accept_connection_error(char* message);
void log_send_all_error(char* message);
void log_receive_amount_of_bytes_error(char* message);
void log_serialized_structure_error(char* message);
void log_pthread_create_for_attend_connections_error();
void log_connection_error(char* message);
void log_expanding_unexpandable_matrix_error();
void log_incorrect_index();
void log_are_not_equals_columns_and_rows_in_squared_matrix();
void log_incorrect_positions();
void log_pthread_create_error(int process_id);
void log_queue_code_not_found(char* queue_name);

#endif //DELIBIRD_GENERAL_LOGS_H
