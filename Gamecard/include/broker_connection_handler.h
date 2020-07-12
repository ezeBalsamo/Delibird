#ifndef DELIBIRD_BROKER_CONNECTION_HANDLER_H
#define DELIBIRD_BROKER_CONNECTION_HANDLER_H

void* initialize_gamecard_broker_connection_handler();
void* subscriber_thread(void* queue_operation_identifier);
void free_gamecard_broker_connection_handler();

#endif //DELIBIRD_BROKER_CONNECTION_HANDLER_H
