#ifndef DELIBIRD_ENTRY_POINT_CONNECTION_BUILDER_H
#define DELIBIRD_ENTRY_POINT_CONNECTION_BUILDER_H

#include <commons/collections/list.h>
#include "../../Utils/include/common_structures.h"

void initialize_entry_point_connection_builder();
t_request* safe_request();
char* process_ip();
char* process_port();
int ack_timeout();

void free_entry_point_connection_builder();

#endif //DELIBIRD_ENTRY_POINT_CONNECTION_BUILDER_H
