#include "common_structures.h"

uint32_t amount_of_bytes_of_appeared(appearedPokemonArguments* );

void serialize_appeared_pokemon(appearedPokemonArguments* , void** , uint32_t* );

void serialize(request* , void** , uint32_t* );

void* deserialize(void* );

request* deserialize_appeared_pokemon(void* );


