#include <commons/collections/list.h>
#include "../include/team_serializable_objects.h"
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/general_logs.h"

t_list* team_operations_information;

void initialize_team_serializable_objects(){
    initialize_serializable_objects();
    team_operations_information = list_create();
    list_add(team_operations_information, (void*) serializable_appeared_pokemon());
    list_add(team_operations_information, (void*) serializable_get_pokemon());
    list_add(team_operations_information, (void*) serializable_localized_pokemon());
    list_add(team_operations_information, (void*) serializable_catch_pokemon());
    list_add(team_operations_information, (void*) serializable_caught_pokemon());

    log_succesful_initialize_serializable_objects();
}

void free_team_serializable_objects(){
    list_destroy(team_operations_information);
    free_serializable_objects();
}