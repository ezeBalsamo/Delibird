#include <query_performer.h>
#include <map.h>
#include "appeared_query_performer.h"

t_query_performer* appeared_pokemon_query_performer;

t_query_performer* appeared_query_performer(){
    return appeared_pokemon_query_performer;
}

bool should_process_pokemon_named(char* pokemon_name){
    return true;
}

void appeared_query_performer_function(t_identified_message* correlative_identified_message){

    t_appeared_pokemon* appeared_pokemon = internal_object_in_correlative(correlative_identified_message);

    if(should_process_pokemon_named(appeared_pokemon -> pokemon_name)){

        t_pokemon* pokemon = safe_malloc(sizeof(t_pokemon));
        pokemon -> pokemon_name = appeared_pokemon -> pokemon_name;
        pokemon -> pos_x = appeared_pokemon -> pos_x;
        pokemon -> pos_y = appeared_pokemon -> pos_y;

        load_pokemon_in_matrix(pokemon);
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