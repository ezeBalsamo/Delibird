#ifndef DELIBIRD_GAMECARD_QUERY_PERFORMER_H
#define DELIBIRD_GAMECARD_QUERY_PERFORMER_H

#include "../../Utils/include/common_structures.h"

typedef struct Gamecard_query_performer{
    bool (*can_be_handled_function) (uint32_t operation);
    void (*perform_function) (t_identified_message* message);
}t_gamecard_query_performer;

void free_query_performer();
void initialize_query_performer();
void gamecard_query_perform(t_request* request);

#endif //DELIBIRD_GAMECARD_QUERY_PERFORMER_H
