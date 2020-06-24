#ifndef DELIBIRD_TEAM_QUERY_PERFORMERS_H
#define DELIBIRD_TEAM_QUERY_PERFORMERS_H

#include <stdbool.h>
#include <stdint.h>
#include "../../Utils/include/common_structures.h"

typedef struct Query_performer{
    bool (*can_handle_function) (uint32_t operation);
    void (*perform_function) (t_identified_message* correlative_identified_message);
}t_query_performer;

void initialize_team_query_performers();

void query_perform(t_request* request);

void free_team_query_performers();

#endif //DELIBIRD_TEAM_QUERY_PERFORMERS_H
