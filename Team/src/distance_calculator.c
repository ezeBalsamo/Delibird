#include "distance_calculator.h"
#include "../../Utils/include/t_list_extension.h"
#include <stdlib.h>

uint32_t distance_between(t_localizable_object* localizable_object, t_localizable_object* another_localizable_object){
    uint32_t x_distance = abs(localizable_object -> pos_x - another_localizable_object -> pos_x);
    uint32_t y_distance = abs(localizable_object -> pos_y - another_localizable_object -> pos_y);

    return x_distance + y_distance;
}

t_trainer_thread_context* trainer_thread_context_closest_to_localizable_object(t_list* trainer_thread_contexts, t_localizable_object* localizable_object){

    t_trainer_thread_context* seed_trainer_thread_context = list_first(trainer_thread_contexts);

    void* _closest_trainer_thread_context(t_trainer_thread_context* trainer_thread_context, t_trainer_thread_context* another_trainer_thread_context){

        uint32_t trainer_distance = distance_between(trainer_thread_context -> localizable_trainer, localizable_object);
        uint32_t another_trainer_distance = distance_between(another_trainer_thread_context -> localizable_trainer, localizable_object);

        return trainer_distance < another_trainer_distance ? trainer_thread_context : another_trainer_thread_context;
    }

    return list_fold(trainer_thread_contexts, seed_trainer_thread_context, (void * (*)(void *, void *)) _closest_trainer_thread_context);
}

t_targetable_object* targetable_pokemon_closest_to_localizable_trainer(t_list* targetable_pokemons, t_localizable_object* localizable_trainer){

    t_targetable_object* seed_targetable_pokemon = list_first(targetable_pokemons);

    void* _closest_targetable_pokemon(t_targetable_object* targetable_pokemon, t_targetable_object* another_targetable_pokemon){

        t_localizable_object* localizable_pokemon = targetable_pokemon -> localizable_pokemon;
        t_localizable_object* another_localizable_pokemon = another_targetable_pokemon -> localizable_pokemon;

        uint32_t localizable_pokemon_distance = distance_between(localizable_pokemon, localizable_trainer);
        uint32_t another_localizable_pokemon_distance = distance_between(another_localizable_pokemon, localizable_trainer);

        return localizable_pokemon_distance < another_localizable_pokemon_distance ? targetable_pokemon : another_targetable_pokemon;
    }

    return list_fold(targetable_pokemons, seed_targetable_pokemon, (void * (*)(void *, void *)) _closest_targetable_pokemon);
}

t_distance_between_two* closest_trainer_thread_context_to_trainer_thread_context(t_list* trainer_thread_contexts, t_trainer_thread_context* trainer_thread_context_to_compare){

    t_localizable_object* localizable_trainer_to_compare = trainer_thread_context_to_compare -> localizable_trainer;
    t_trainer_thread_context* seed_trainer_thread_context = list_first(trainer_thread_contexts);

    void* _closest(t_trainer_thread_context* trainer_thread_context, t_trainer_thread_context* another_trainer_thread_context){

        t_localizable_object* localizable_trainer = trainer_thread_context -> localizable_trainer;
        t_localizable_object* another_localizable_trainer = another_trainer_thread_context -> localizable_trainer;

        uint32_t localizable_trainer_distance = distance_between(localizable_trainer, localizable_trainer_to_compare);
        uint32_t another_localizable_trainer_distance = distance_between(another_localizable_trainer, localizable_trainer_to_compare);

        return localizable_trainer_distance < another_localizable_trainer_distance ? trainer_thread_context : another_trainer_thread_context;
    }

    t_trainer_thread_context* closest_trainer_thread_context =
            list_fold(trainer_thread_contexts, seed_trainer_thread_context, (void * (*)(void *, void *)) _closest);

    uint32_t distance = distance_between(localizable_trainer_to_compare, closest_trainer_thread_context -> localizable_trainer);

    t_distance_between_two* distance_between_two = safe_malloc(sizeof(t_distance_between_two));
    distance_between_two -> first_object = trainer_thread_context_to_compare;
    distance_between_two -> second_object = closest_trainer_thread_context;
    distance_between_two -> distance = distance;

    return distance_between_two;
}

void consider_adding_as_the_closest_pair_in(t_list* distances_of_pair_of_trainer_thread_contexts, t_distance_between_two* distance_between_two_to_compare){

    t_distance_between_two* candidate_distance_between_two = list_remove_first(distances_of_pair_of_trainer_thread_contexts);

    if(distance_between_two_to_compare -> distance < candidate_distance_between_two -> distance){
        list_add(distances_of_pair_of_trainer_thread_contexts, distance_between_two_to_compare);
        free(candidate_distance_between_two);
    }else{
        list_add(distances_of_pair_of_trainer_thread_contexts, candidate_distance_between_two);
        free(distance_between_two_to_compare);
    }
}

t_distance_between_two* closest_pair_of_trainer_thread_contexts_according_to(t_list* trainer_thread_contexts){

    t_list* duplicated_trainer_thread_contexts = list_duplicate(trainer_thread_contexts);
    t_list* distances_of_pair_of_trainer_thread_contexts = list_create();

    for(int i = 0; i < (list_size(trainer_thread_contexts) - 1); i++){

        t_trainer_thread_context* trainer_thread_context = list_remove_first(duplicated_trainer_thread_contexts);
        t_distance_between_two* distance_between_two =
                closest_trainer_thread_context_to_trainer_thread_context(duplicated_trainer_thread_contexts, trainer_thread_context);

        if(list_is_empty(distances_of_pair_of_trainer_thread_contexts)){
            list_add(distances_of_pair_of_trainer_thread_contexts, distance_between_two);
        }else{
            consider_adding_as_the_closest_pair_in(distances_of_pair_of_trainer_thread_contexts, distance_between_two);
        }
    }

    assert_only_one_in(distances_of_pair_of_trainer_thread_contexts);
    list_destroy(duplicated_trainer_thread_contexts);

    t_distance_between_two* distance_of_pair_of_trainer_thread_contexts =
            list_remove_first(distances_of_pair_of_trainer_thread_contexts);

    list_destroy(distances_of_pair_of_trainer_thread_contexts);
    return distance_of_pair_of_trainer_thread_contexts;
}

t_list* closest_pair_of_trainer_thread_contexts_in(t_list* trainer_thread_contexts){

    t_distance_between_two* distance_of_pair_of_trainer_thread_contexts =
            closest_pair_of_trainer_thread_contexts_according_to(trainer_thread_contexts);

    t_list* closest_pair_of_trainer_thread_contexts = list_create();

    list_add(closest_pair_of_trainer_thread_contexts, distance_of_pair_of_trainer_thread_contexts -> first_object);
    list_add(closest_pair_of_trainer_thread_contexts, distance_of_pair_of_trainer_thread_contexts -> second_object);

    free(distance_of_pair_of_trainer_thread_contexts);
    return closest_pair_of_trainer_thread_contexts;
}

t_localizable_object* average_trainer_thread_context_position_for(t_list* trainer_thread_contexts){

    uint32_t summarized_x_position = 0;
    uint32_t summarized_y_position = 0;
    uint32_t trainer_thread_contexts_quantity = list_size(trainer_thread_contexts);

    void _sum_each_position(t_trainer_thread_context* trainer_thread_context){
        t_localizable_object* localizable_trainer = trainer_thread_context -> localizable_trainer;
        summarized_x_position += localizable_trainer -> pos_x;
        summarized_y_position += localizable_trainer -> pos_y;
    }

    list_iterate(trainer_thread_contexts, (void (*)(void *)) _sum_each_position);

    uint32_t average_x_position = summarized_x_position / trainer_thread_contexts_quantity;
    uint32_t average_y_position = summarized_y_position / trainer_thread_contexts_quantity;

    t_localizable_object* average_position = safe_malloc(sizeof(t_localizable_object));
    average_position -> object = trainer_thread_contexts;
    average_position -> pos_x = average_x_position;
    average_position -> pos_y = average_y_position;

    return average_position;
}

t_trainer_thread_context* closest_trainer_thread_context_to_trainer_thread_contexts(t_list* trainer_thread_contexts_to_search, t_list* trainer_thread_contexts_to_compare){

    t_localizable_object* average_trainer_thread_context_position =
            average_trainer_thread_context_position_for(trainer_thread_contexts_to_compare);

    t_trainer_thread_context* closest_trainer_thread_context =
            trainer_thread_context_closest_to_localizable_object(trainer_thread_contexts_to_search, average_trainer_thread_context_position);

    free(average_trainer_thread_context_position);

    return closest_trainer_thread_context;
}