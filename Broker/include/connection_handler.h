#ifndef DELIBIRD_TEAM_CONNECTION_HANDLER_H
#define DELIBIRD_TEAM_CONNECTION_HANDLER_H

uint32_t get_message_id();
void update_message_id();
sem_t* get_mutex_id();
void* initialize_connection_handler();

#endif //DELIBIRD_TEAM_CONNECTION_HANDLER_H
