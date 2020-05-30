#include <query_performer.h>
#include "localized_query_performer.h"
#include <stdlib.h>
#include "../../Utils/include/common_structures.h"
#include "goal_calculator.h"

t_query_performer *localized_pokemon_query_performer;

t_query_performer* localized_query_performer(){
    return localized_pokemon_query_performer;
}

void localized_query_performer_function(void* request_structure){
    t_localized_pokemon* localized_pokemon = (t_localized_pokemon*)request_structure;
    //TODO: logica query_performer localized
        //verificar msg repetido
        // idem appeared pero para cada par de coordenadas donde esta el pokemon
}

bool localized_query_performer_can_handle(uint32_t operation){
    return operation == LOCALIZED_POKEMON;
}

void initialize_localized_query_performer(){
    localized_pokemon_query_performer = malloc(sizeof(t_query_performer));
    localized_pokemon_query_performer->can_handle_function = localized_query_performer_can_handle;
    localized_pokemon_query_performer->perform_function = localized_query_performer_function;

}

