#include <trainers_parser.h>
#include <goal_calculator.h>
#include <team_manager.h>
#include <pokemon_occurrences.h>
#include <stdlib.h>
#include <trainer_threads.h>
#include <commons/string.h>
#include <team_logs_manager.h>
#include <dispatcher.h>
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/pthread_wrapper.h"

t_list* localized_trainers;
t_list* global_goal;

pthread_mutex_t localized_trainers_mutex;
pthread_mutex_t global_goal_mutex;

bool global_goal_accomplished;

void* initialize_team_manager(){

    global_goal_accomplished = false;
    safe_mutex_initialize(&localized_trainers_mutex);
    safe_mutex_initialize(&global_goal_mutex);

    localized_trainers = parsed_trainers();
    global_goal = team_global_goal_according_to(localized_trainers);
    initialize_pokemon_occurrences();
    initialize_trainer_threads();

    return NULL;
}

bool are_equal_trainers(t_trainer* trainer, t_trainer* another_trainer){
    return trainer -> sequential_number == another_trainer -> sequential_number;
}

t_pokemon_goal* global_goal_of(char* pokemon_name){
    bool _are_equals(void* pokemon_goal){
        t_pokemon_goal* cast_pokemon_goal = (t_pokemon_goal*) pokemon_goal;
        return string_equals_ignore_case(cast_pokemon_goal -> pokemon_name, pokemon_name);
    }

    t_pokemon_goal* pokemon_goal_found = list_find(global_goal, _are_equals);

    if(!pokemon_goal_found){
        log_pokemon_not_belonging_to_global_goal_error_for(pokemon_name);
        free_system();
    }

    return pokemon_goal_found;
}

uint32_t amount_required_of(char* pokemon_name){

    t_pokemon_goal* pokemon_goal = global_goal_of(pokemon_name);
    return pokemon_goal -> quantity;
}

bool global_goal_contains(char* pokemon_name){

    bool _are_equals(void* pokemon_goal){
        t_pokemon_goal* cast_pokemon_goal = (t_pokemon_goal*) pokemon_goal;
        return string_equals_ignore_case(cast_pokemon_goal -> pokemon_name, pokemon_name);
    }

    return list_any_satisfy(global_goal, _are_equals);
}

void update_global_goal_after_caught_of(char* pokemon_name){
    pthread_mutex_lock(&global_goal_mutex);
    t_pokemon_goal* pokemon_goal = global_goal_of(pokemon_name);
    pokemon_goal -> quantity--;
    pthread_mutex_unlock(&global_goal_mutex);
}

void update_current_pokemons_after_caught(t_localizable_object* localizable_trainer, char* pokemon_name){
    t_trainer* trainer = localizable_trainer -> object;

    pthread_mutex_lock(&localized_trainers_mutex);
    list_add(trainer -> current_pokemons, string_duplicate(pokemon_name));
    pthread_mutex_unlock(&localized_trainers_mutex);

    update_global_goal_after_caught_of(pokemon_name);
}

void assert_equals_size_between_trainers_and_finished_trainer_thread_contexts(){
    int finished_amount = finished_trainer_thread_contexts_amount();
    if(list_size(localized_trainers) != finished_amount){
        log_not_matching_trainers_amount_with_finished_thread_contexts_amount_on_global_goal_accomplished_error();
        free_system();
    }
}

void assert_there_are_no_more_global_goal_requirements(){

    bool _amount_required_is_zero(void* pokemon_goal){
        t_pokemon_goal* cast_pokemon_goal = (t_pokemon_goal*) pokemon_goal;
        return cast_pokemon_goal -> quantity == 0;
    }

    bool global_goal_is_accomplished = list_all_satisfy(global_goal, _amount_required_is_zero);

    if(!global_goal_is_accomplished){
        log_global_goal_not_consistent_with_trainers_requirements_error();
        free_system();
    }
}

void consider_global_goal_accomplished(){

    bool _has_no_requirements_left(void* localized_trainer){
        t_localizable_object* cast_localized_trainer = (t_localizable_object*) localized_trainer;
        t_trainer* trainer = cast_localized_trainer -> object;
        t_list* trainer_requirements = requirements_of(trainer);
        bool has_no_requeriments_left = list_is_empty(trainer_requirements);

        list_destroy(trainer_requirements);
        return has_no_requeriments_left;
    }

    global_goal_accomplished =
            list_all_satisfy(localized_trainers, _has_no_requirements_left);

    if(global_goal_accomplished){
        assert_all_trainer_thread_contexts_have_finished();
        assert_equals_size_between_trainers_and_finished_trainer_thread_contexts();
        assert_there_are_no_more_global_goal_requirements();
        log_global_goal_accomplished();
    }
}

bool is_global_goal_accomplished(){
    return global_goal_accomplished;
}

void with_trainers_do(void (*closure) (t_localizable_object*)){
    pthread_mutex_lock(&localized_trainers_mutex);
    list_iterate(localized_trainers, (void (*)(void *)) closure);
    pthread_mutex_unlock(&localized_trainers_mutex);
}

void with_global_goal_do(void (*closure) (t_pokemon_goal*)){
    pthread_mutex_lock(&global_goal_mutex);
    list_iterate(global_goal, (void (*)(void *)) closure);
    pthread_mutex_unlock(&global_goal_mutex);
}

void free_localizable_trainer(t_localizable_object* localizable_trainer){
    t_trainer* trainer = localizable_trainer -> object;
    list_destroy_and_destroy_elements(trainer -> required_pokemons, free);
    list_destroy_and_destroy_elements(trainer -> current_pokemons, free);
    free(trainer);
    free(localizable_trainer);
}

void free_team_manager(){
    list_destroy_and_destroy_elements(global_goal, (void (*)(void *)) free);
    list_destroy_and_destroy_elements(localized_trainers, (void (*)(void *)) free_localizable_trainer);
    pthread_mutex_destroy(&localized_trainers_mutex);
    pthread_mutex_destroy(&global_goal_mutex);
}