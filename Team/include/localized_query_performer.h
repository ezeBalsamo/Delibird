#ifndef DELIBIRD_LOCALIZED_QUERY_PERFORMER_H
#define DELIBIRD_LOCALIZED_QUERY_PERFORMER_H

#include "team_query_performers.h"

typedef struct Get_response{
    uint32_t response_id;
    char* pokemon_name;
}t_get_response;

void initialize_localized_query_performer();

t_query_performer* localized_query_performer();
void get_pokemon_sent_successfully(t_get_response* get_response);

void free_localized_query_performer();

#endif //DELIBIRD_LOCALIZED_QUERY_PERFORMER_H
