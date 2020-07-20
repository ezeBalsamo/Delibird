#ifndef DELIBIRD_CAUGHT_QUERY_PERFORMER_H
#define DELIBIRD_CAUGHT_QUERY_PERFORMER_H

#include "team_query_performers.h"

void initialize_caught_query_performer();
t_query_performer* caught_query_performer();
t_identified_message* arrival_identified_message_of(int ack);

#endif //DELIBIRD_CAUGHT_QUERY_PERFORMER_H
