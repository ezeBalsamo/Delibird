#ifndef TP_2020_1C_EL_KUELGUE_QUEUE_MESSAGE_MANAGER_H
#define TP_2020_1C_EL_KUELGUE_QUEUE_MESSAGE_MANAGER_H

#include <stdint.h>
#include <commons/collections/queue.h>
#include "../../Utils/include/common_structures.h"
#include "publish_message_mode.h"

void initialize_queue_message_manager();

t_queue* get_queue_of(uint32_t operation);

t_request* create_request_id(t_message_status* message_status);

void push_to_queue(t_message_status* message_status);

void publish(t_message_status* message_status);

void free_queue_message_manager();

#endif //TP_2020_1C_EL_KUELGUE_QUEUE_MESSAGE_MANAGER_H
