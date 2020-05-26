#ifndef DELIBIRD_QUERY_PERFORMER_H
#define DELIBIRD_QUERY_PERFORMER_H
#include <stdbool.h>
#include <stdint.h>

typedef struct Query_performer{
    bool (*can_handle_function) (uint32_t operation);
    void (*perform_function) (void* structure);
}t_query_performer;

t_query_performer* query_performer_handle(uint32_t operation);

#endif //DELIBIRD_QUERY_PERFORMER_H
