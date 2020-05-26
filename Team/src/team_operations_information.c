#include <commons/collections/list.h>
#include "../include/team_operations_information.h"
#include "../../Utils/include/operations_information.h"

t_list* team_operations_information;

void initialize_team_operations_information(){
    initialize_operations_information();
    team_operations_information = list_create();
    list_add(team_operations_information, (void*) appeared_pokemon_operation_information());
    list_add(team_operations_information, (void*) get_pokemon_operation_information());
    list_add(team_operations_information, (void*) localized_pokemon_operation_information());
    list_add(team_operations_information, (void*) catch_pokemon_operation_information());
    list_add(team_operations_information, (void*) caught_pokemon_operation_information());
}

void free_team_operations_information(){
    list_clean(team_operations_information);
    list_destroy(team_operations_information);
    free_operations_information();
}