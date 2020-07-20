#include <team_configuration_manager.h>
#include <trainer_thread_context_execution_cycle.h>
#include "catch_action.h"
#include "../../Utils/include/socket.h"
#include <stdlib.h>
#include <team_logs_manager.h>
#include <event_notifier.h>
#include <waiting_actions.h>
#include <caught_query_performer.h>

t_catch_pokemon* catch_pokemon_for(t_trainer_thread_context* trainer_thread_context){

    t_catch_action* catch_action = internal_thread_action_in(trainer_thread_context);
    t_localizable_object* localizable_pokemon = catch_action -> localizable_pokemon;

    t_catch_pokemon* catch_pokemon = safe_malloc(sizeof(t_catch_pokemon));
    catch_pokemon -> pokemon_name = localizable_pokemon -> object;
    catch_pokemon -> pos_x = localizable_pokemon -> pos_x;
    catch_pokemon -> pos_y = localizable_pokemon -> pos_y;

    return catch_pokemon;
}

void apply_default_catch_action_for(t_trainer_thread_context* trainer_thread_context){
    t_catch_action* catch_action = internal_thread_action_in(trainer_thread_context);
    log_succesfully_caught_due_to_failed_communication_with_broker(catch_action -> localizable_pokemon);

    catch_action_completed_successfully_by(trainer_thread_context);
}

void search_if_message_has_already_arrived(t_trainer_thread_context* trainer_thread_context, int ack){

     t_identified_message* identified_message = arrival_identified_message_of(ack);

     if(identified_message != NULL){
         t_waiting_catch_response_action* waiting_catch_response_action =
                 internal_thread_action_in(trainer_thread_context);

         t_caught_pokemon* caught_pokemon = internal_object_in(identified_message);

         waiting_catch_response_action -> caught_succeeded = caught_pokemon -> caught_status;
         safe_sem_post(&trainer_thread_context -> semaphore);
     }
}

void apply_catch_action_when_connection_success(t_request* request,
                                                t_connection_information* connection_information,
                                                t_trainer_thread_context* trainer_thread_context){

    int ack =
            serialize_and_send_structure_and_wait_for_ack(
                    request,
                    connection_information -> socket_fd,
                    ack_timeout());

    notify_with_argument(EXECUTION_CYCLE_CONSUMED_BY_TRAINER,
                         trainer_from_thread_context(trainer_thread_context));

    if (ack == FAILED_ACK){
        apply_default_catch_action_for(trainer_thread_context);
    }else{
        catch_action_blocked_in_wait_of_response(trainer_thread_context, ack);
        search_if_message_has_already_arrived(trainer_thread_context, ack);
    }
}

void catch_action_execution_function(t_trainer_thread_context* trainer_thread_context){
    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = CATCH_POKEMON;
    request -> structure = catch_pokemon_for(trainer_thread_context);
    request -> sanitizer_function = free;

    t_connection_information* connection_information = connect_to(broker_ip(), broker_port());

    if(connection_information -> connection_was_succesful) {
        apply_catch_action_when_connection_success(request, connection_information, trainer_thread_context);
    } else {
        apply_default_catch_action_for(trainer_thread_context);
    }

    free_and_close_connection_information(connection_information);
    free_request(request);
}

t_thread_action* catch_thread_action_for(t_localizable_object* localizable_pokemon){

    t_catch_action* catch_action = safe_malloc(sizeof(t_catch_action));
    catch_action -> localizable_pokemon = localizable_pokemon;

    t_thread_action* thread_action = new_thread_action();
    thread_action -> request -> operation = CATCH;
    thread_action -> request -> structure = catch_action;
    thread_action -> request -> sanitizer_function = free;
    thread_action -> execution_function = (void (*)(void *)) catch_action_execution_function;

    return thread_action;
}