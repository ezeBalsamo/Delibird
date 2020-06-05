#include <team_configuration_manager.h>
#include <trainer_thread_context_execution_cycle.h>
#include "catch_action.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/socket.h"

t_catch_action* catch_action_for(t_localizable_object* localizable_pokemon){

    t_catch_action* catch_action = safe_malloc(sizeof(t_catch_action));
    catch_action -> localizable_pokemon = localizable_pokemon;

    return catch_action;
}

t_catch_pokemon* catch_pokemon_for(t_trainer_thread_context* trainer_thread_context){

    t_catch_action* catch_action = trainer_thread_context -> thread_action;
    t_localizable_object* localizable_pokemon = catch_action -> localizable_pokemon;

    t_catch_pokemon* catch_pokemon = safe_malloc(sizeof(t_catch_pokemon));
    catch_pokemon -> pokemon_name = localizable_pokemon -> object;
    catch_pokemon -> pos_x = localizable_pokemon -> pos_x;
    catch_pokemon -> pos_y = localizable_pokemon -> pos_y;

    return catch_pokemon;
}


void catch_action_execution_function(t_trainer_thread_context* trainer_thread_context){
    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = CATCH_POKEMON;
    request -> structure = catch_pokemon_for(trainer_thread_context);

    t_connection_information* connection_information = connect_to(broker_ip(), broker_port());

    if(!connection_information -> connection_was_succesful) {
        catch_action_completed_by(trainer_thread_context);
    } else {
        //TODO: traer implementación de nico de serialize_send_structure_and_wait_for_ack(request, socket_fd);
    }
}