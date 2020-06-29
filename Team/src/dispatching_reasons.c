#include <movement_action.h>
#include <commons/string.h>
#include "../include/dispatching_reasons.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/t_list_extension.h"
#include <stdlib.h>
#include <team_logs_manager.h>
#include <catch_action.h>
#include <waiting_actions.h>
#include <team_pretty_prints.h>
#include <deadlock_solver.h>
#include <identified_exchanges_provider.h>

char* catch_pokemon_reason_for(t_trainer_thread_context* trainer_thread_context){
    t_catch_action* catch_action = internal_thread_action_in(trainer_thread_context);
    t_localizable_object* localizable_pokemon = catch_action -> localizable_pokemon;

    char* localizable_pokemon_string = localizable_pokemon_as_string(localizable_pokemon);
    char* reason = string_from_format("Atrapar %s.", localizable_pokemon_string);
    free(localizable_pokemon_string);

    return reason;
}

char* movement_reason_according_to(t_localizable_object* destiny_object){

    char* printable_destiny_object = localizable_object_as_string(destiny_object);
    char* reason = string_from_format("Moverse en dirección a %s.", printable_destiny_object);
    free(printable_destiny_object);

    return reason;
}

char* movement_reason_for(t_trainer_thread_context* trainer_thread_context){
    t_movement_action* movement_action = internal_thread_action_in(trainer_thread_context);
    t_localizable_object* destiny_object = movement_action -> destiny_object;

    return movement_reason_according_to(destiny_object);
}

char* waiting_catch_response_reason_for(t_trainer_thread_context* trainer_thread_context){

    t_waiting_catch_response_action* waiting_catch_response_action =
            internal_thread_action_in(trainer_thread_context);

    t_localizable_object* localizable_pokemon = waiting_catch_response_action -> localizable_pokemon;

    char* localizable_pokemon_string = localizable_pokemon_as_string(localizable_pokemon);
    char* reason =
            string_from_format("Esperar el resultado de atrapar %s con el id de mensaje %d.",
                    localizable_pokemon_string, waiting_catch_response_action -> message_id);
    free(localizable_pokemon_string);

    return reason;
}

char* waiting_for_more_pokemons_reason(){
    //Las demás razones son más complejas y requieren limpieza
    return string_from_format("%s", "Esperar a que aparezcan más pokemones.");
}

char* waiting_for_exchange_reason_for(t_trainer_thread_context* trainer_thread_context){

    t_waiting_for_exchange_action* waiting_for_deadlock_resolution_action =
            internal_thread_action_in(trainer_thread_context);

    char* required_pokemons_not_caught_as_string =
            pokemon_names_as_string(waiting_for_deadlock_resolution_action->required_pokemons_not_caught);

    char* pokemons_in_excess_as_string =
            pokemon_names_as_string(waiting_for_deadlock_resolution_action->pokemons_in_excess);

    char* reason =
            string_from_format("Esperar la resolución de un intercambio.\n"
                               "Pokemones requeridos y no capturados: %s.\nPokemones en exceso: %s.",
                               required_pokemons_not_caught_as_string, pokemons_in_excess_as_string);

    free(required_pokemons_not_caught_as_string);
    free(pokemons_in_excess_as_string);
    return reason;
}

char* exchange_reason(){
    t_list* identified_exchanges_in_process = current_identified_exchanges_in_process();
    t_identified_exchange* identified_exchange = list_first(identified_exchanges_in_process);

    t_trainer* second_party_trainer =
            second_party_localizable_trainer_in(identified_exchange) -> object;
    char* printable_second_party_localizable_trainer =
            trainer_as_string(second_party_trainer);

    char* reason = string_from_format("Realizar uno o más intercambios con %s",
            printable_second_party_localizable_trainer);

    free(printable_second_party_localizable_trainer);
    return reason;
}

char* quantum_consumed_reason(){
    //Se aloca memoria en lugar de devolver el string porque los
    //otros motivos de logueo de schedule la alocan y la función
    //que loguea asume eso y realiza un free

    return string_from_format("%s", "Quantum agotado");
}

char* preemption_reason(){
    //TODO: sumarle la lógica para cuando SJF CD
    return quantum_consumed_reason();
}

char* thread_action_reason_for(t_trainer_thread_context* trainer_thread_context){
    uint32_t thread_action_type = trainer_thread_context -> thread_action -> request -> operation;
    char* reason = NULL;

    switch (thread_action_type) {
        case MOVEMENT:
            reason = movement_reason_for(trainer_thread_context);
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
        case WAITING_FOR_EXCHANGE:
            reason = waiting_for_exchange_reason_for(trainer_thread_context);
            break;
        case EXCHANGE:
            reason = exchange_reason();
            break;
        default:
            log_unknown_thread_action_type_error();
            free_system();
            break;
    }
    return reason;
}

