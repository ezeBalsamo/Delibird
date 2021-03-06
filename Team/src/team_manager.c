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
#include <metrics.h>
#include <event_notifier.h>
#include <team_configuration_manager.h>
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/pthread_wrapper.h"
#include <unistd.h>

t_list* localized_trainers;
t_list* global_goal;

pthread_mutex_t localized_trainers_mutex;
pthread_mutex_t global_goal_mutex;
extern sem_t localized_trainers_created;

bool global_goal_is_accomplished;

void wait_the_time_delay(){
    sleep(time_delay());
}

static void subscribe_to_events(){
    subscribe_to_event_doing(EXECUTION_CYCLE_CONSUMED, wait_the_time_delay);
    subscribe_to_event_doing(EXECUTION_CYCLE_CONSUMED_BY_TRAINER, wait_the_time_delay);
}

void* initialize_team_manager(){

    global_goal_is_accomplished = false;
    safe_mutex_initialize(&localized_trainers_mutex);
    safe_mutex_initialize(&global_goal_mutex);

    localized_trainers = parsed_trainers();
    safe_sem_post(&localized_trainers_created);

    global_goal = team_global_goal_according_to(localized_trainers);
    subscribe_to_events();

    initialize_metrics();
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
    safe_mutex_lock(&localized_trainers_mutex);
    function();
    safe_mutex_unlock(&localized_trainers_mutex);
}

void handling_global_goal_concurrency_do(void (*function) ()){
    safe_mutex_lock(&global_goal_mutex);
    function();
    safe_mutex_unlock(&global_goal_mutex);
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

char* safe_pokemon_remove(t_list* pokemons, char* pokemon_name_to_find){

    bool _is_equals(char* pokemon_name_to_compare){
        return string_equals_ignore_case(pokemon_name_to_compare, pokemon_name_to_find);
    }

    char* pokemon_removed = list_remove_by_condition(pokemons, (bool (*)(void*)) _is_equals);

    if(!pokemon_removed){
        log_pokemon_not_found_error_for(pokemon_name_to_find);
        free_system();
    }

    return pokemon_removed;
}

void trade_trainers_pokemons(char* first_party_pokemon_name,
                                t_localizable_object* first_party_localizable_trainer,
                                char* second_party_pokemon_name,
                                t_localizable_object* second_party_localizable_trainer){

    t_trainer* first_party_trainer = first_party_localizable_trainer -> object;
    t_trainer* second_party_trainer = second_party_localizable_trainer -> object;

    void _trade(){
        char* pokemon_for_second_party_trainer =
                safe_pokemon_remove(first_party_trainer -> current_pokemons, first_party_pokemon_name);

        char* pokemon_for_first_party_trainer =
                safe_pokemon_remove(second_party_trainer -> current_pokemons, second_party_pokemon_name);

        list_add(first_party_trainer -> current_pokemons, pokemon_for_first_party_trainer);
        list_add(second_party_trainer -> current_pokemons, pokemon_for_second_party_trainer);
    }

    handling_localized_trainers_concurrency_do(_trade);
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
        dump_metrics();
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

    list_iterate(global_goal, (void (*)(void *)) closure);
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
    free_metrics();
    free_pokemon_occurrences();
    free_trainer_threads();
    free_team_query_performers();

    list_destroy_and_destroy_elements(global_goal, (void (*)(void *)) free);
    list_destroy_and_destroy_elements(localized_trainers, (void (*)(void *)) free_localizable_trainer);

    safe_mutex_destroy(&localized_trainers_mutex);
    safe_mutex_destroy(&global_goal_mutex);
}