#ifndef DELIBIRD_TEAM_MANAGER_H
#define DELIBIRD_TEAM_MANAGER_H

#include <stdint.h>
#include <commons/collections/list.h>

typedef struct Trainer{
    uint32_t sequential_number;
    uint32_t pos_x;
    uint32_t pos_y;
    t_list* current_pokemons;
    t_list* desired_pokemons;
}t_trainer;

typedef struct PokemonGoal{
    char* pokemon_name;
    uint32_t quantity;
}t_pokemon_goal;

void initialize_team_manager();
void calculate_global_goal();
t_list *get_trainer_objective(t_trainer *trainer, t_list *team_objective);
void unify_pokemon_goals(t_list* pokemon_goals_flattened);

void with_trainers_do(void (*closure) (t_trainer*));
void with_global_goal_do(void (*closure) (t_pokemon_goal*));

t_list* trainers_x_positions();
t_list* trainers_y_positions();

#endif //DELIBIRD_TEAM_MANAGER_H
