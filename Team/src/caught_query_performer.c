#include <team_query_performers.h>
#include <trainer_threads.h>
#include <waiting_actions.h>
#include <dispatcher.h>
#include <team_logs_manager.h>
#include "caught_query_performer.h"

t_query_performer* caught_pokemon_query_performer;

t_query_performer* caught_query_performer(){
    return caught_pokemon_query_performer;
}

bool is_waiting_catch_response(t_trainer_thread_context* trainer_thread_context){
    t_thread_action* thread_action = trainer_thread_context -> thread_action;
    return thread_action -> request -> operation == WAITING_CATCH_RESPONSE;
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
        sem_post(&trainer_thread_context_found -> semaphore);
    }else{
        log_message_id_not_required(message_id);
    }
}

bool caught_query_performer_can_handle(uint32_t operation){
    return operation == CAUGHT_POKEMON;
}

void initialize_caught_query_performer(){
    caught_pokemon_query_performer = safe_malloc(sizeof(t_query_performer));
    caught_pokemon_query_performer -> can_handle_function = caught_query_performer_can_handle;
    caught_pokemon_query_performer -> perform_function = caught_query_performer_function;
}

