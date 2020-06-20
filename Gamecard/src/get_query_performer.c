#include "get_query_performer.h"
#include "gamecard_query_performer.h"
#include "../../Utils/include/common_structures.h"
#include <stdlib.h>
#include <stdio.h>

t_gamecard_query_performer *get_pokemon_query_performer;

t_gamecard_query_performer* get_query_performer(){
    return get_pokemon_query_performer;
}

void get_query_performer_function(t_identified_message* identified_message){
    printf("Se recibio el mensaje GET_POKEMON con id = %d\n", identified_message -> message_id);
    //TODO: Implementar lógica

}

bool get_query_performer_can_handle(uint32_t operation){
    return operation == GET_POKEMON;
}

void initialize_get_query_performer(){
    get_pokemon_query_performer = malloc(sizeof(t_gamecard_query_performer));
    get_pokemon_query_performer->can_be_handled_function = get_query_performer_can_handle;
    get_pokemon_query_performer->perform_function = get_query_performer_function;
}
