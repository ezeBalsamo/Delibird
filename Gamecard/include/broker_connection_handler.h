#ifndef DELIBIRD_BROKER_CONNECTION_HANDLER_H
#define DELIBIRD_BROKER_CONNECTION_HANDLER_H

void sleep_for(int reconnection_time_in_seconds);
void initialize_gamecard_broker_connection_handler();
void performer_thread(t_request* deserialized_request);
void* subscriber_thread(void* queue_operation_identifier);
void free_gamecard_broker_connection_handler();

#endif //DELIBIRD_BROKER_CONNECTION_HANDLER_H
