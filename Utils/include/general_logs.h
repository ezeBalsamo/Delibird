#ifndef DELIBIRD_GENERAL_LOGS_H
#define DELIBIRD_GENERAL_LOGS_H

#include <commons/log.h>
#include "common_structures.h"

void log_succesful_start_up();
void log_successful_execution();
void log_successful_clean_up();
void log_succesful_initialize_config_manager();
void log_succesful_initialize_pretty_printer();
void log_succesful_initialize_serializable_objects();
void log_syscall_error(char* message_error);
void log_send_all_error(int sent_bytes, int amount_of_bytes);
void log_expanding_unexpandable_matrix_error();
void log_invalid_index_error();
void log_are_not_equals_columns_and_rows_in_squared_matrix_error();
void log_invalid_positions_error();
void log_pthread_create_error(unsigned int process_id);
void log_queue_name_not_found_error(uint32_t queue_code);
void log_queue_code_not_found_error(char* queue_name);
void log_printable_object_not_found_error();
void log_about_to_send_request(t_request* request);
void log_request_sent(t_request* request);
void log_request_received_with(t_log* logger, t_request* request);
void log_unknown_chained_evaluation_type_error();
void log_garbage_to_stop_considering_that_not_found_error();
void log_failed_ack_error();
void log_succesful_suscription_to(uint32_t operation_queue);
void log_errorful_not_existing_log(char* log_name);
void log_expected_to_have_only_one_element_error();

void log_list_invalid_index_access(int index, int another_index,t_list* self);
#endif //DELIBIRD_GENERAL_LOGS_H
