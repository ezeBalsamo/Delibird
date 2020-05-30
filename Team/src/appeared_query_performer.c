#include <query_performer.h>
#include <map.h>
#include "appeared_query_performer.h"
#include <stdlib.h>
#include "../../Utils/include/common_structures.h"
#include "goal_calculator.h"

t_query_performer *appeared_pokemon_query_performer;

t_query_performer* appeared_query_performer(){
    return appeared_pokemon_query_performer;
}

void appeared_query_performer_function(void* request_structure){
    t_appeared_pokemon* appeared_pokemon = (t_appeared_pokemon*)request_structure;
    if (global_goal_contains(appeared_pokemon->pokemon_name)) {
        t_pokemon *pokemon = malloc(sizeof(t_pokemon));
        pokemon->pokemon_name = appeared_pokemon->pokemon_name;
        pokemon->pos_x = appeared_pokemon->pos_x;
        pokemon->pos_y = appeared_pokemon->pos_y;

        load_pokemon_in_matrix(pokemon);
        //llamar dispatcher... dispatch_trainer_to_catch(pokemon);
    }
}

bool appeared_query_performer_can_handle(uint32_t operation){
    return operation == APPEARED_POKEMON;
}

void initialize_appeared_query_performer(){
    appeared_pokemon_query_performer = malloc(sizeof(t_query_performer));
    appeared_pokemon_query_performer->can_handle_function = appeared_query_performer_can_handle;
    appeared_pokemon_query_performer->perform_function = appeared_query_performer_function;

}
