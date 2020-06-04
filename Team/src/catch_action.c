#include "catch_action.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/configuration_manager.h"

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

char* broker_ip(){
    return config_get_string_at("BROKER_IP");
}
char* broker_port(){
    return config_get_string_at("BROKER_PORT");
}

void catch_action_execution_function(t_trainer_thread_context* trainer_thread_context){
    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = CATCH_POKEMON;
    request -> structure = catch_pokemon_for(trainer_thread_context);

    t_connection_information* connection_information = connect_to(broker_ip(), broker_port());

    if(!connection_information -> connection_was_succesful) {
        //TODO: lógica de comportamiento por default
    } else {
        //TODO: traer implementación de nico de serialize_send_structure_and_wait_for_ack(request, socket_fd);
    }
}