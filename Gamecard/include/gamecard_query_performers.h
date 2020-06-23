#ifndef DELIBIRD_GAMECARD_QUERY_PERFORMERS_H
#define DELIBIRD_GAMECARD_QUERY_PERFORMERS_H

#include "../../Utils/include/common_structures.h"

typedef struct Gamecard_query_performer{
    bool (*can_be_handled_function) (uint32_t operation);
    void (*perform_function) (t_identified_message* identified_message);
}t_gamecard_query_performer;

void free_gamecard_query_performers();
void initialize_gamecard_query_performers();
void gamecard_query_perform(t_request* request);

#endif //DELIBIRD_GAMECARD_QUERY_PERFORMERS_H
