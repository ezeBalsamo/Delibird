#ifndef DELIBIRD_TEAM_MANAGER_H
#define DELIBIRD_TEAM_MANAGER_H

#include <stdint.h>
#include <commons/collections/list.h>

typedef struct Trainer{
    uint32_t pos_x;
    uint32_t pos_y;
    t_list* current_pokemons;
    t_list* desired_pokemons;
}t_trainer;

typedef struct PokemonObjective{
    char* pokemon_name;
    int32_t amount_to_catch;
}t_pokemon_objective;

void initialize_team_manager();
void *get_team_global_objective();
t_list *get_trainer_objective(t_trainer *trainer);
t_list *get_actual_trainer_objective(t_trainer *trainer);
#endif //DELIBIRD_TEAM_MANAGER_H
