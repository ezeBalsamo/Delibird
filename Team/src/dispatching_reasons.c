#include <movement_action.h>
#include <commons/string.h>
#include "../include/dispatching_reasons.h"
#include "../../Utils/include/garbage_collector.h"
#include <stdlib.h>
#include <team_logs_manager.h>
#include <catch_action.h>
#include <waiting_actions.h>

char* localizable_pokemon_as_string(t_localizable_object* localizable_pokemon){
    char* pokemon_name = localizable_pokemon -> object;
    char* localizable_pokemon_string =
            string_from_format("%s, ubicado en (%d, %d)",
                               pokemon_name,
                               localizable_pokemon -> pos_x,
                               localizable_pokemon -> pos_y);

    return localizable_pokemon_string;
}

char* catch_pokemon_reason_for(t_trainer_thread_context* trainer_thread_context){
    t_catch_action* catch_action = internal_thread_action_in(trainer_thread_context);
    t_localizable_object* localizable_pokemon = catch_action -> localizable_pokemon;

    char* localizable_pokemon_string = localizable_pokemon_as_string(localizable_pokemon);
    char* reason = string_from_format("Atrapar un %s.", localizable_pokemon_string);
    free(localizable_pokemon_string);

    return reason;
}

char* move_to_pokemon_reason_for(t_trainer_thread_context* trainer_thread_context){
    t_movement_action* movement_action = internal_thread_action_in(trainer_thread_context);
    t_localizable_object* localizable_pokemon = movement_action -> destiny_object;

    char* localizable_pokemon_string = localizable_pokemon_as_string(localizable_pokemon);
    char* reason = string_from_format("Moverse en dirección a un %s.", localizable_pokemon_string);
    free(localizable_pokemon_string);

    return reason;
}

char* waiting_catch_response_reason_for(t_trainer_thread_context* trainer_thread_context){

    t_waiting_catch_response_action* waiting_catch_response_action =
            internal_thread_action_in(trainer_thread_context);

    t_localizable_object* localizable_pokemon =
            waiting_catch_response_action -> catch_action -> localizable_pokemon;

    char* localizable_pokemon_string = localizable_pokemon_as_string(localizable_pokemon);
    char* reason =
            string_from_format("Esperar el resultado de atrapar un %s con el id de mensaje %d.",
                    localizable_pokemon_string, waiting_catch_response_action -> message_id);
    free(localizable_pokemon_string);

    return reason;
}

char* waiting_for_more_pokemons_reason(){
    //Las demás razones son más complejas y requieren limpieza
    return string_from_format("%s","Esperar a que aparezcan más pokemones.");
}

char* thread_action_reason_for(t_trainer_thread_context* trainer_thread_context){
    uint32_t thread_action_type = trainer_thread_context -> thread_action -> request -> operation;
    char* reason = NULL;

    switch (thread_action_type) {
        case MOVEMENT:
            reason = move_to_pokemon_reason_for(trainer_thread_context);
            break;
        case CATCH:
            reason = catch_pokemon_reason_for(trainer_thread_context);
            break;
        case WAITING_CATCH_RESPONSE:
            reason = waiting_catch_response_reason_for(trainer_thread_context);
            break;
        case WAITING_FOR_MORE_POKEMONS:
            reason = waiting_for_more_pokemons_reason();
            break;
        default:
            log_unknown_thread_action_type_error();
            free_system();
            break;
    }

    return reason;
}

