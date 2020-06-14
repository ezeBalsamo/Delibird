#include "../include/waiting_actions.h"
#include <stdlib.h>
#include <dispatching_reasons.h>

void waiting_catch_response_action_execution_function(t_trainer_thread_context* trainer_thread_context){
    //TODO
}

t_thread_action* waiting_catch_response_thread_action_for(t_catch_action* catch_action, int message_id){

    t_waiting_catch_response_action* waiting_catch_response_action =
            safe_malloc(sizeof(t_waiting_catch_response_action));

    waiting_catch_response_action -> catch_action = catch_action;
    waiting_catch_response_action -> message_id = message_id;

    t_thread_action* thread_action = new_thread_action();
    thread_action -> request -> operation = WAITING_CATCH_RESPONSE;
    thread_action -> request -> structure = waiting_catch_response_action;
    thread_action -> request -> sanitizer_function = free;
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