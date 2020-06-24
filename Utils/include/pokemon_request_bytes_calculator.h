#ifndef DELIBIRD_POKEMON_REQUEST_BYTES_CALCULATOR_H
#define DELIBIRD_POKEMON_REQUEST_BYTES_CALCULATOR_H

uint32_t size_to_allocate_for(t_request* request);

uint32_t amount_of_bytes_of_new(t_new_pokemon* new_pokemon);
uint32_t amount_of_bytes_of_appeared(t_appeared_pokemon* appeared_pokemon);
uint32_t amount_of_bytes_of_get(t_get_pokemon* get_pokemon);
uint32_t amount_of_bytes_of_localized(t_localized_pokemon * localized_pokemon);
uint32_t amount_of_bytes_of_catch(t_catch_pokemon* catch_pokemon);
uint32_t amount_of_bytes_of_subscribe_me(t_subscribe_me* subscribe_me);

#endif //DELIBIRD_POKEMON_REQUEST_BYTES_CALCULATOR_H
