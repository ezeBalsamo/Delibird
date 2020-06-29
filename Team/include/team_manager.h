#ifndef DELIBIRD_TEAM_MANAGER_H
#define DELIBIRD_TEAM_MANAGER_H

#include <stdint.h>
#include <commons/collections/list.h>
#include <semaphore.h>

enum Localizable_object_types{
    POKEMON, TRAINER
};

typedef struct Localizable_object{
    int32_t pos_x;
    int32_t pos_y;
    void* object;
    uint32_t type;
}t_localizable_object;

typedef struct Targetable_object{
    bool is_being_targeted;
    bool should_be_targeted;
    t_localizable_object* localizable_pokemon;
}t_targetable_object;

typedef struct Trainer{
    int sequential_number;
    t_list* current_pokemons;
    t_list* required_pokemons;
}t_trainer;

typedef struct Pokemon_goal{
    char* pokemon_name;
    uint32_t quantity;
}t_pokemon_goal;

void* initialize_team_manager();

void with_trainers_do(void (*closure) (t_localizable_object*));
void with_global_goal_do(void (*closure) (t_pokemon_goal*));
int trainers_amount();

bool are_equal_trainers(t_trainer* trainer, t_trainer* another_trainer);

bool global_goal_contains(char* pokemon_name);
uint32_t amount_required_of(char* pokemon_name);

void update_current_pokemons_after_caught(t_localizable_object* localizable_trainer, char* pokemon_name);
void exchange_trainers_pokemons(char* first_party_pokemon_name,
                                t_localizable_object* first_party_localizable_trainer,
                                char* second_party_pokemon_name, t_localizable_object*
                                second_party_localizable_trainer);

void all_trainer_threads_context_have_finished();

bool is_global_goal_accomplished();

void free_team_manager();

#endif //DELIBIRD_TEAM_MANAGER_H
