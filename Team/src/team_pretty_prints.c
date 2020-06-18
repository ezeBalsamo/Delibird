#include <commons/string.h>
#include "team_pretty_prints.h"

char* localizable_pokemon_as_string(t_localizable_object* localizable_pokemon){
    char* pokemon_name = localizable_pokemon -> object;
    char* localizable_pokemon_string =
            string_from_format("%s, ubicado en (%d, %d)",
                               pokemon_name,
                               localizable_pokemon -> pos_x,
                               localizable_pokemon -> pos_y);

    return localizable_pokemon_string;
}