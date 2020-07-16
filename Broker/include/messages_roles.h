#ifndef DELIBIRD_MESSAGES_ROLES_H
#define DELIBIRD_MESSAGES_ROLES_H

#include <stdbool.h>
#include "../../Utils/include/common_structures.h"
#include "connection_handler.h"

typedef struct MessageRole{
    bool (*can_handle_function) (uint32_t operation);
    void (*attending_message_function) (t_connection_deserialization_information* connection_deserialization_information);
}t_message_role;

void initialize_messages_roles();
void attend_with_message_role(t_connection_deserialization_information* connection_deserialization_information);
void free_message_identifier();

#endif //DELIBIRD_MESSAGES_ROLES_H
