#include <commons/string.h>
#include <trainer_threads.h>
#include <team_logs_manager.h>
#include "team_pretty_prints.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/t_list_extension.h"

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
            free_system();
            break;
    }

    return state_as_string;
}

char* pokemon_names_as_string(t_list* pokemon_names){

    char* pokemon_names_as_string = string_new();

    for(int i = 0; i < list_size(pokemon_names); i++){
        char* pokemon_name = list_get(pokemon_names, i);
        string_append(&pokemon_names_as_string, pokemon_name);

        if (i + 1 != list_size(pokemon_names)){
            string_append(&pokemon_names_as_string, ", ");
        }
    }

    return pokemon_names_as_string;
}