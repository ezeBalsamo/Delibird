#include <team_query_performers.h>
#include <pokemon_occurrences.h>
#include <team_logs_manager.h>
#include <pthread.h>
#include "appeared_query_performer.h"

t_query_performer* appeared_pokemon_query_performer;

extern pthread_mutex_t pokemon_occurrence_status_by_name_mutex;

t_query_performer* appeared_query_performer(){
    return appeared_pokemon_query_performer;
}

void add_new_occurrence_according_to(t_appeared_pokemon* appeared_pokemon){
    char* pokemon_name = appeared_pokemon -> pokemon_name;
    uint32_t pos_x = appeared_pokemon -> pos_x;
    uint32_t pos_y = appeared_pokemon -> pos_y;

    t_targetable_object* targetable_pokemon = targetable_pokemon_according_to(pokemon_name, pos_x, pos_y);
    new_occurrence_of(targetable_pokemon);
}

void consider_as_new_if_necessary_occurrence_of(t_appeared_pokemon* appeared_pokemon){

    char* pokemon_name = appeared_pokemon -> pokemon_name;

    pthread_mutex_lock(&pokemon_occurrence_status_by_name_mutex);

    if(!exists_previous_occurrence_of(pokemon_name)){
        mark_occurrence_status_of(pokemon_name);
    }

    pthread_mutex_unlock(&pokemon_occurrence_status_by_name_mutex);
}

void appeared_query_performer_function(t_identified_message* correlative_identified_message){

    t_appeared_pokemon* appeared_pokemon = internal_object_in_correlative(correlative_identified_message);
    char* pokemon_name = appeared_pokemon -> pokemon_name;

    if(global_goal_contains(pokemon_name)){
        consider_as_new_if_necessary_occurrence_of(appeared_pokemon);
        add_new_occurrence_according_to(appeared_pokemon);
    }else{
        log_appeared_pokemon_not_necessary_for_global_goal(pokemon_name);
    }
}

bool appeared_query_performer_can_handle(uint32_t operation){
    return operation == APPEARED_POKEMON;
}

void initialize_appeared_query_performer(){
    appeared_pokemon_query_performer = safe_malloc(sizeof(t_query_performer));
    appeared_pokemon_query_performer -> can_handle_function = appeared_query_performer_can_handle;
    appeared_pokemon_query_performer -> perform_function = appeared_query_performer_function;
}