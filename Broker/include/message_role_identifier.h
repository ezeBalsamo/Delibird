#ifndef DELIBIRD_MESSAGE_ROLE_IDENTIFIER_H
#define DELIBIRD_MESSAGE_ROLE_IDENTIFIER_H

#include <stdbool.h>
#include "../../Utils/include/common_structures.h"

typedef struct MessageRoleIdentifier{
    bool (*can_handle_function) (uint32_t operation);
    void (*attending_message_function) (t_connection_request* connection_request);
}t_message_role_identifier;

void initialize_message_role_identifier();
void attend_with_message_role_identifier(t_connection_request* connection_request);
void free_message_identifier();

#endif //DELIBIRD_MESSAGE_ROLE_IDENTIFIER_H
