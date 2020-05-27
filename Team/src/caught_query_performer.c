#include <query_performer.h>
#include "caught_query_performer.h"
#include <stdlib.h>
#include "../../Utils/include/common_structures.h"
#include "goal_calculator.h"

t_query_performer *caught_pokemon_query_performer;

t_query_performer* caught_query_performer(){
    return caught_pokemon_query_performer;
}
void caught_query_performer_function(void* request_structure){
    t_caught_pokemon* caught_pokemon = (t_caught_pokemon*)request_structure;
    //TODO: logica query_performer caught
    //chequear si yo tire catch de este id, y ademas traer la info de ese mensaje
        //porque necesito saber q pokemon es para persistir el obj
    //si lo capture, lo saco de la matriz y persisto eso en el trainer y chequeo si esta pa exit
}
bool caught_query_performer_can_handle(uint32_t operation){
    return operation == CAUGHT_POKEMON;
}

void initialize_caught_query_performer(){
    caught_pokemon_query_performer = malloc(sizeof(t_query_performer));
    caught_pokemon_query_performer->can_handle_function = caught_query_performer_can_handle;
    caught_pokemon_query_performer->perform_function = caught_query_performer_function;

}

