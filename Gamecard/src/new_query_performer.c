#include "new_query_performer.h"
#include "gamecard_query_performer.h"
#include "../../Utils/include/common_structures.h"
#include <stdlib.h>
#include <stdio.h>

t_gamecard_query_performer *new_pokemon_query_performer;

t_gamecard_query_performer* new_query_performer(){
    return new_pokemon_query_performer;
}

void new_query_performer_function(t_identified_message* identified_message){
    printf("Se recibio el mensaje NEW_POKEMON con id = %d\n", identified_message -> message_id);
    //TODO: Implementar lógica

}

bool new_query_performer_can_handle(uint32_t operation){
    return operation == NEW_POKEMON;
}

void initialize_new_query_performer(){
    new_pokemon_query_performer = malloc(sizeof(t_gamecard_query_performer));
    new_pokemon_query_performer->can_be_handled_function = new_query_performer_can_handle;
    new_pokemon_query_performer->perform_function = new_query_performer_function;
}