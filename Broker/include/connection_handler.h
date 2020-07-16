#ifndef DELIBIRD_TEAM_CONNECTION_HANDLER_H
#define DELIBIRD_TEAM_CONNECTION_HANDLER_H

typedef struct Connection_deserialization_information{
    t_deserialization_information* deserialization_information;
    int socket_fd;
}t_connection_deserialization_information;

void initialize_connection_handler();

uint32_t update_and_get_message_id();

void free_connection_handler();
void free_connection_deserialization_information(t_connection_deserialization_information* connection_deserialization_information);

#endif //DELIBIRD_TEAM_CONNECTION_HANDLER_H
