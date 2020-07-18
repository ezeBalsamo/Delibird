#ifndef DELIBIRD_TEAM_CONNECTION_HANDLER_H
#define DELIBIRD_TEAM_CONNECTION_HANDLER_H

#include "../../Utils/include/serialization_interface.h"

typedef struct Connection_deserialization_information{
    t_deserialization_information* deserialization_information;
    int socket_fd;
}t_connection_deserialization_information;

void execute_main_thread();

uint32_t update_and_get_message_id();

void free_connection_handler();
void free_connection_deserialization_information(t_connection_deserialization_information* connection_deserialization_information);
void free_main_thread();

#endif //DELIBIRD_TEAM_CONNECTION_HANDLER_H
