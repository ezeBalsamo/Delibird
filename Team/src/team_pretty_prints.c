#include <commons/string.h>
#include <trainer_threads.h>
#include <team_logs_manager.h>
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

char* localizable_trainer_as_string(t_localizable_object* localizable_trainer){
    t_trainer* trainer = localizable_trainer -> object;
    char* localizable_trainer_as_string =
            string_from_format("entrenador %d, ubicado en (%d, %d)",
                                trainer -> sequential_number,
                                localizable_trainer -> pos_x,
                                localizable_trainer -> pos_y);

    return localizable_trainer_as_string;
}

char* state_as_string(uint32_t state){
    char* state_as_string = NULL;

    switch (state) {
        case NEW:
            state_as_string = "cola de nuevos";
            break;
        case READY:
            state_as_string = "cola de listos";
            break;
        case EXECUTE:
            state_as_string = "ejecución";
            break;
        case BLOCKED:
            state_as_string = "cola de bloqueados";
            break;
        case FINISHED:
            state_as_string = "cola de finalizados";
            break;
        default:
            log_invalid_state_error();
    }

    return state_as_string;
}