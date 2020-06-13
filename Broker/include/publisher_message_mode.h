#ifndef DELIBIRD_PUBLISHER_MESSAGE_MODE_H
#define DELIBIRD_PUBLISHER_MESSAGE_MODE_H

#include "../../Utils/include/common_structures.h"
#include "message_role_identifier.h"

t_message_role_identifier* publisher_mode();

void initialize_publisher_message_mode();

void allocate_message(t_identified_message* identified_message);

#endif //DELIBIRD_PUBLISHER_MESSAGE_MODE_H
