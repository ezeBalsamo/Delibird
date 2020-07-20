#include <team_query_performers.h>
#include <trainer_threads.h>
#include <waiting_actions.h>
#include <dispatcher.h>
#include <team_logs_manager.h>
#include <stdlib.h>
#include "caught_query_performer.h"
#include "../../Utils/include/garbage_collector.h"

t_query_performer* caught_pokemon_query_performer;
t_list* caught_identified_messages_responses;

t_query_performer* caught_query_performer(){
    return caught_pokemon_query_performer;
}

bool is_waiting_catch_response(t_trainer_thread_context* trainer_thread_context){
    return internal_thread_action_type_in(trainer_thread_context) == WAITING_CATCH_RESPONSE;
}

t_trainer_thread_context* blocked_trainer_thread_context_waiting_for(int message_id){
    t_list* trainer_thread_contexts = non_schedulable_blocked_trainer_thread_contexts();

    t_list* waiting_catch_response_trainer_thread_contexts =
            list_filter(trainer_thread_contexts, (bool (*)(void *)) is_waiting_catch_response);

    bool _is_waiting_this_message_id(void* trainer_thread_context){
        t_trainer_thread_context* cast_trainer_thread_context =
                (t_trainer_thread_context*) trainer_thread_context;

        t_waiting_catch_response_action* waiting_catch_response_action =
                internal_thread_action_in(cast_trainer_thread_context);

        return waiting_catch_response_action -> message_id == message_id;
    }

    t_trainer_thread_context* trainer_thread_context_found =
            list_find(waiting_catch_response_trainer_thread_contexts, _is_waiting_this_message_id);

    list_destroy(trainer_thread_contexts);
    list_destroy(waiting_catch_response_trainer_thread_contexts);

    return trainer_thread_context_found;
}

t_identified_message* arrival_identified_message_of(int ack){

    bool _is_for(t_identified_message* identified_message){
        return identified_message -> message_id == ack;
    }

    return list_find(caught_identified_messages_responses, _is_for);
}

void caught_query_performer_function(t_identified_message* correlative_identified_message){

    t_identified_message* identified_message = internal_object_in(correlative_identified_message);
    uint32_t message_id = identified_message -> message_id;

    t_trainer_thread_context* trainer_thread_context_found =
            blocked_trainer_thread_context_waiting_for(message_id);

    if(trainer_thread_context_found){
        t_waiting_catch_response_action* waiting_catch_response_action =
                internal_thread_action_in(trainer_thread_context_found);

        t_caught_pokemon* caught_pokemon = internal_object_in(identified_message);

        waiting_catch_response_action -> caught_succeeded = caught_pokemon -> caught_status;
        safe_sem_post(&trainer_thread_context_found -> semaphore);
    }else{
        list_add(caught_identified_messages_responses, identified_message);
    }
}

bool caught_query_performer_can_handle(uint32_t operation){
    return operation == CAUGHT_POKEMON;
}

void destroy_responses(){
    list_destroy_and_destroy_elements(caught_identified_messages_responses, free);
}

void initialize_caught_query_performer(){
    caught_identified_messages_responses = list_create();
    caught_pokemon_query_performer = safe_malloc(sizeof(t_query_performer));
    caught_pokemon_query_performer -> can_handle_function = caught_query_performer_can_handle;
    caught_pokemon_query_performer -> perform_function = caught_query_performer_function;

    consider_as_garbage(caught_identified_messages_responses, destroy_responses);
}

