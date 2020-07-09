#include <trainers_parser.h>
#include <goal_calculator.h>
#include <team_manager.h>
#include <pokemon_occurrences.h>
#include <stdlib.h>
#include <trainer_threads.h>
#include <commons/string.h>
#include <team_logs_manager.h>
#include <dispatcher.h>
#include <team_query_performers.h>
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/pthread_wrapper.h"

t_list* localized_trainers;
t_list* global_goal;

pthread_mutex_t localized_trainers_mutex;
pthread_mutex_t global_goal_mutex;
extern sem_t localized_trainers_created;

bool global_goal_is_accomplished;

void* initialize_team_manager(){

    global_goal_is_accomplished = false;
    safe_mutex_initialize(&localized_trainers_mutex);
    safe_mutex_initialize(&global_goal_mutex);

    localized_trainers = parsed_trainers();
    sem_post(&localized_trainers_created);

    global_goal = team_global_goal_according_to(localized_trainers);

    initialize_team_query_performers();
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

    bool _are_equals(t_pokemon_goal* pokemon_goal){
        return string_equals_ignore_case(pokemon_goal -> pokemon_name, pokemon_name);
    }

    return list_any_satisfy(global_goal, (bool (*)(void*)) _are_equals);
}

void handling_localized_trainers_concurrency_do(void (*function) ()){
    pthread_mutex_lock(&localized_trainers_mutex);
    function();
    pthread_mutex_unlock(&localized_trainers_mutex);
}

void handling_global_goal_concurrency_do(void (*function) ()){
    pthread_mutex_lock(&global_goal_mutex);
    function();
    pthread_mutex_unlock(&global_goal_mutex);
}

void decrement_global_goal_quantity_after_caught_of(char* pokemon_name){

    void _decrement(){
        t_pokemon_goal* pokemon_goal = global_goal_of(pokemon_name);
        pokemon_goal -> quantity--;
    }
    handling_global_goal_concurrency_do(_decrement);
}

void update_current_pokemons_after_caught(t_localizable_object* localizable_trainer, char* pokemon_name){
    t_trainer* trainer = localizable_trainer -> object;

    void _add(){
        list_add(trainer -> current_pokemons, string_duplicate(pokemon_name));
    }

    handling_localized_trainers_concurrency_do(_add);
    decrement_global_goal_quantity_after_caught_of(pokemon_name);
}

void exchange_trainers_pokemons(char* first_party_pokemon_name,
                                t_localizable_object* first_party_localizable_trainer,
                                char* second_party_pokemon_name, t_localizable_object*
                                second_party_localizable_trainer){

    t_trainer* first_party_trainer = first_party_localizable_trainer -> object;
    t_trainer* second_party_trainer = second_party_localizable_trainer -> object;

    bool _is_equals_to_first(char* pokemon_name){
        return string_equals_ignore_case(pokemon_name, first_party_pokemon_name);
    }

    bool _is_equals_to_second(char* pokemon_name){
        return string_equals_ignore_case(pokemon_name, second_party_pokemon_name);
    }

    void _exchange(){
        list_remove_by_condition(first_party_trainer -> current_pokemons, (bool (*)(void *)) _is_equals_to_first);
        list_add(first_party_trainer -> current_pokemons, second_party_pokemon_name);
        list_remove_by_condition(second_party_trainer -> current_pokemons,(bool (*)(void *)) _is_equals_to_second);
        list_add(second_party_trainer -> current_pokemons, string_duplicate(first_party_pokemon_name));
    }

    handling_localized_trainers_concurrency_do(_exchange);
}

void assert_there_are_no_more_global_goal_requirements(){

    bool _has_no_requirements_left(t_pokemon_goal* pokemon_goal){
        return pokemon_goal -> quantity == 0;
    }

    bool global_goal_completed =
            list_all_satisfy(global_goal, (bool (*)(void*)) _has_no_requirements_left);

    if(!global_goal_completed){
        log_global_goal_not_consistent_with_trainers_requirements_error();
        free_system();
    }
}

void all_trainer_threads_context_have_finished(){

    bool _has_no_requirements_left(void* localized_trainer){
        t_localizable_object* cast_localized_trainer = (t_localizable_object*) localized_trainer;
        t_trainer* trainer = cast_localized_trainer -> object;
        return has_accomplished_own_goal(trainer);
    }

    global_goal_is_accomplished =
            list_all_satisfy(localized_trainers, _has_no_requirements_left);

    if(global_goal_is_accomplished){
        assert_there_are_no_more_global_goal_requirements();
        log_global_goal_accomplished();
    }
    else{
        log_expected_global_goal_to_be_accomplished_error();
    }
}

bool is_global_goal_accomplished(){
    return global_goal_is_accomplished;
}

void with_trainers_do(void (*closure) (t_localizable_object*)){

    void _iterate(){
        list_iterate(localized_trainers, (void (*)(void *)) closure);
    }
    handling_localized_trainers_concurrency_do(_iterate);
}

void with_global_goal_do(void (*closure) (t_pokemon_goal*)){

    void _iterate(){
        list_iterate(global_goal, (void (*)(void *)) closure);
    }
    handling_global_goal_concurrency_do(_iterate);
}

int trainers_amount(){
    return list_size(localized_trainers);
}

void free_localizable_trainer(t_localizable_object* localizable_trainer){
    t_trainer* trainer = localizable_trainer -> object;
    list_destroy_and_destroy_elements(trainer -> required_pokemons, free);
    list_destroy_and_destroy_elements(trainer -> current_pokemons, free);
    free(trainer);
    free(localizable_trainer);
}

void free_team_manager(){
    free_pokemon_occurrences();
    free_trainer_threads();
    free_team_query_performers();

    list_destroy_and_destroy_elements(global_goal, (void (*)(void *)) free);
    list_destroy_and_destroy_elements(localized_trainers, (void (*)(void *)) free_localizable_trainer);

    pthread_mutex_destroy(&localized_trainers_mutex);
    pthread_mutex_destroy(&global_goal_mutex);
}