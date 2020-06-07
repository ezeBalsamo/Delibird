#include <query_performer.h>
#include <map.h>
#include <stdlib.h>
#include "appeared_query_performer.h"

t_query_performer* appeared_pokemon_query_performer;
sem_t targetable_status;

t_query_performer* appeared_query_performer(){
    return appeared_pokemon_query_performer;
}

bool should_be_targeted_pokemon_named(char* pokemon_name){
    uint32_t amount_required = amount_required_of(pokemon_name);
    uint32_t ocurrences = occurrences_amount_in_map_of(pokemon_name);
    return ocurrences < amount_required;
}

t_targetable_object* targetable_pokemon_according_to(t_appeared_pokemon* appeared_pokemon){

    t_localizable_object* localizable_pokemon = safe_malloc(sizeof(t_localizable_object));
    localizable_pokemon -> pos_x = appeared_pokemon -> pos_x;
    localizable_pokemon -> pos_y = appeared_pokemon -> pos_y;
    localizable_pokemon -> object = appeared_pokemon -> pokemon_name;

    sem_wait(&targetable_status);
    bool should_be_targeted = should_be_targeted_pokemon_named(appeared_pokemon -> pokemon_name);;
    sem_post(&targetable_status);

    t_targetable_object* targetable_pokemon = safe_malloc(sizeof(t_targetable_object));
    targetable_pokemon -> is_being_targeted = should_be_targeted;
    targetable_pokemon -> localizable_pokemon = localizable_pokemon;

    return targetable_pokemon;
}

void appeared_query_performer_function(t_identified_message* correlative_identified_message){

    t_appeared_pokemon* appeared_pokemon = internal_object_in_correlative(correlative_identified_message);

    if(global_goal_contains(appeared_pokemon -> pokemon_name)){

        t_targetable_object* targetable_pokemon = targetable_pokemon_according_to(appeared_pokemon);
        free(appeared_pokemon);
        load_pokemon_in_map(targetable_pokemon);
    }
}

bool appeared_query_performer_can_handle(uint32_t operation){
    return operation == APPEARED_POKEMON;
}

void initialize_appeared_query_performer(){
    sem_initialize(&targetable_status);

    appeared_pokemon_query_performer = safe_malloc(sizeof(t_query_performer));
    appeared_pokemon_query_performer -> can_handle_function = appeared_query_performer_can_handle;
    appeared_pokemon_query_performer -> perform_function = appeared_query_performer_function;
}