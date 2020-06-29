#include "catch_query_performer.h"
#include "gamecard_query_performers.h"
#include "../../Utils/include/common_structures.h"
#include <stdlib.h>
#include <stdio.h>

t_gamecard_query_performer *catch_pokemon_query_performer;

t_gamecard_query_performer* catch_query_performer(){
    return catch_pokemon_query_performer;
}

t_identified_message* catch_query_performer_function(t_identified_message* identified_message){
    printf("Se recibio el mensaje CATCH_POKEMON con id = %d\n", identified_message -> message_id);
    //TODO: Implementar lógica

}

bool catch_query_performer_can_handle(uint32_t operation){
    return operation == CATCH_POKEMON;
}

void initialize_catch_query_performer(){
    catch_pokemon_query_performer = malloc(sizeof(t_gamecard_query_performer));
    catch_pokemon_query_performer->can_be_handled_function = catch_query_performer_can_handle;
    catch_pokemon_query_performer->perform_function = catch_query_performer_function;
}

