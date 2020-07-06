#ifndef DELIBIRD_TEAM_BROKER_CONNECTION_HANDLER_H
#define DELIBIRD_TEAM_BROKER_CONNECTION_HANDLER_H

void* initialize_team_broker_connection_handler();
void* subscriber_thread(void* queue_operation_identifier);
void free_team_broker_connection_handler();

#endif //DELIBIRD_TEAM_BROKER_CONNECTION_HANDLER_H
