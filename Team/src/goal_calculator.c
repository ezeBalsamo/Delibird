#include "goal_calculator.h"
#include <commons/string.h>
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/common_structures.h"
#include <stdlib.h>

t_list* requirements_of(t_trainer* trainer){
    return list_difference(trainer -> required_pokemons, trainer -> current_pokemons,
                           (bool (*)(void *, void *)) string_equals_ignore_case);
}

bool can_catch_pokemons(t_trainer* trainer){
    return list_size(trainer -> current_pokemons) < list_size(trainer -> required_pokemons);
}

t_list* goal_of(t_trainer* trainer){
    t_list* trainer_goal = list_create();
    t_list* trainer_required_pokemons = requirements_of(trainer);

    for (int i = 0; i < list_size(trainer_required_pokemons); i++) {

        char* pokemon_name = (char *) list_get(trainer_required_pokemons, i);

        bool _is_equal_pokemon(void *pokemon_objective){
            return string_equals_ignore_case(((t_pokemon_goal*) pokemon_objective) -> pokemon_name, pokemon_name);
        }

        t_pokemon_goal* pokemon_goal = list_find(trainer_goal, _is_equal_pokemon);

        if (pokemon_goal == NULL) {
            pokemon_goal = safe_malloc(sizeof(t_pokemon_goal));
            pokemon_goal -> pokemon_name = pokemon_name;
            pokemon_goal -> quantity = 1;
            list_add(trainer_goal, (void*) pokemon_goal);
        } else {
            uint32_t amount = pokemon_goal -> quantity;
            pokemon_goal -> quantity = amount++;
        }
    }

    list_destroy(trainer_required_pokemons);
    return trainer_goal;
}

t_list* unified_pokemon_goals(t_list* flattened_team_pokemon_goals) {

    t_list* global_goal = list_create();

    for (int i = 0; i < list_size(flattened_team_pokemon_goals); i++) {

        t_pokemon_goal* trainer_pokemon_goals = list_get(flattened_team_pokemon_goals, i);

        bool _is_equal_pokemon(void* pokemon_goal){
            return string_equals_ignore_case(((t_pokemon_goal*) pokemon_goal) -> pokemon_name,
                                             trainer_pokemon_goals -> pokemon_name);
        }

        t_pokemon_goal* global_pokemon_goal = list_find(global_goal, _is_equal_pokemon);

        if (global_pokemon_goal == NULL) {
            global_pokemon_goal = safe_malloc(sizeof(t_pokemon_goal));
            global_pokemon_goal -> pokemon_name = trainer_pokemon_goals -> pokemon_name;
            global_pokemon_goal -> quantity = trainer_pokemon_goals -> quantity;
            list_add(global_goal, (void *) global_pokemon_goal);
        } else {
            uint32_t amount_to_sum = global_pokemon_goal -> quantity;
            global_pokemon_goal -> quantity = amount_to_sum + trainer_pokemon_goals -> quantity;
        }
    }

    return global_goal;
}

t_list* team_global_goal_according_to(t_list* localizable_trainers){
    t_list* team_pokemon_goals = list_create();

    for (int i = 0; i < list_size(localizable_trainers); i++){
        t_localizable_object* localizable_trainer = list_get(localizable_trainers, i);
        t_trainer* trainer = localizable_trainer -> object;
        t_list* trainer_pokemon_goals = goal_of(trainer);
        list_add(team_pokemon_goals, trainer_pokemon_goals);
    }

    t_list* flattened_team_pokemon_goals = list_flat(team_pokemon_goals);
    t_list* team_global_goal = unified_pokemon_goals(flattened_team_pokemon_goals);

    list_of_lists_destroy_and_destroy_elements(team_pokemon_goals, (void (*)(void *)) free);
    list_destroy(flattened_team_pokemon_goals);

    return team_global_goal;
}