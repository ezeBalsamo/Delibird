#include "../include/waiting_actions.h"
#include <stdlib.h>
#include <dispatching_reasons.h>
#include <trainer_thread_context_execution_cycle.h>
#include <map.h>

void waiting_catch_response_action_execution_function(t_trainer_thread_context* trainer_thread_context){
    t_waiting_catch_response_action* waiting_catch_response_action =
            internal_thread_action_in(trainer_thread_context);

    t_localizable_object* localizable_pokemon =
            waiting_catch_response_action -> catch_action -> localizable_pokemon;

    bool caught_succeeded = waiting_catch_response_action -> caught_succeeded;

    free_thread_action(trainer_thread_context -> thread_action);

    if(caught_succeeded){
        trainer_thread_context -> thread_action = catch_thread_action_for(localizable_pokemon);
        catch_action_completed_by(trainer_thread_context);
    }else{
        remove_pokemon_from_map(localizable_pokemon);
        prepare_for_waiting_for_more_pokemons_action(trainer_thread_context);
    }
}

void free_waiting_catch_response_action(t_waiting_catch_response_action* waiting_catch_response_action){
    free(waiting_catch_response_action -> catch_action);
    free(waiting_catch_response_action);
}

t_thread_action* waiting_catch_response_thread_action_for(t_catch_action* catch_action, int message_id){

    t_waiting_catch_response_action* waiting_catch_response_action =
            safe_malloc(sizeof(t_waiting_catch_response_action));

    waiting_catch_response_action -> catch_action = catch_action;
    waiting_catch_response_action -> message_id = message_id;
    waiting_catch_response_action -> caught_succeeded = false;

    t_thread_action* thread_action = new_thread_action();
    thread_action -> request -> operation = WAITING_CATCH_RESPONSE;
    thread_action -> request -> structure = waiting_catch_response_action;
    thread_action -> request -> sanitizer_function = (void (*)(void *)) free_waiting_catch_response_action;
    thread_action -> execution_function = (void (*)(void *)) waiting_catch_response_action_execution_function;

    return thread_action;
}

t_thread_action* waiting_for_more_pokemons_thread_action(){

    t_thread_action* thread_action = new_thread_action();
    thread_action -> request -> operation = WAITING_FOR_MORE_POKEMONS;
    thread_action -> request -> structure = NULL;
    thread_action -> request -> sanitizer_function = free;

    return thread_action;
}