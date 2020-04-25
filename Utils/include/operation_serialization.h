#include "common_structures.h"

uint32_t amount_of_bytes_of_appeared(appearedPokemonArguments* );

uint32_t amount_of_bytes_of_new(newPokemonArguments* );

uint32_t amount_of_bytes_of_catch(catchPokemonArguments* );

void serialize_appeared_pokemon(appearedPokemonArguments* , void** , uint32_t* );

void serialize_new_pokemon(newPokemonArguments* , void** , uint32_t* );

void serialize_catch_pokemon(catchPokemonArguments* , void** , uint32_t* );

void serialize(request* , void** , uint32_t* );

void* deserialize(void* );

request* deserialize_appeared_pokemon(void* );

request* deserialize_new_pokemon(void* );

request* deserialize_catch_pokemon(void* );


