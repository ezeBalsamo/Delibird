#include <common_structures.h>
#include "pokemon_request_bytes_calculator.h"
#include <string.h>

uint32_t amount_of_bytes_of_new(t_new_pokemon* new_pokemon){
    return sizeof(uint32_t)                                     //Pokemon name length
           + strlen(new_pokemon -> pokemon_name)                //Pokemon name
           + sizeof(uint32_t)                                   //Pos x
           + sizeof(uint32_t)                                   //Pos y
           + sizeof(uint32_t);                                  //Cantidad
}

uint32_t amount_of_bytes_of_appeared(t_appeared_pokemon* appeared_pokemon){
    return sizeof(uint32_t)                                     //Pokemon name length
           + strlen(appeared_pokemon -> pokemon_name)          //Pokemon name
           + sizeof(uint32_t)                                  //Pos x
           + sizeof(uint32_t);                                 //Pos y
}

uint32_t amount_of_bytes_of_get(t_get_pokemon* get_pokemon){
    return sizeof(uint32_t)                                     //Pokemon name length
           + strlen(get_pokemon -> pokemon_name);       	    //Pokemon name
}

uint32_t amount_of_bytes_of_localized(t_localized_pokemon * localized_pokemon){
    return sizeof(uint32_t)                                                 //Pokemon name length
           + strlen(localized_pokemon -> pokemon_name)                      //Pokemon name
           + sizeof(uint32_t)                                               //quantity
           + (list_size(localized_pokemon->positions) *sizeof(uint32_t));   //positions
}

uint32_t amount_of_bytes_of_catch(t_catch_pokemon* catch_pokemon){
    return sizeof(uint32_t)                                     //Pokemon name length
           + strlen(catch_pokemon -> pokemon_name)              //Pokemon name
           + sizeof(uint32_t)                                   //Pos x
           + sizeof(uint32_t);                                  //Pos y
}

uint32_t amount_of_bytes_of_subscribe_me(t_subscribe_me* subscribe_me){
    return sizeof(uint32_t)                                     //Operation queue
           + sizeof(uint32_t)                                  //Process description name length
           + strlen(subscribe_me -> process_description);      //Process description name
}

uint32_t size_to_allocate_for(t_request* request){
    uint32_t size = 0;
    switch (request->operation) {
        case NEW_POKEMON:{
            t_new_pokemon* new_pokemon = (t_new_pokemon*) request->structure;
            return amount_of_bytes_of_new(new_pokemon);
        }
        case LOCALIZED_POKEMON:{
            t_localized_pokemon* localized_pokemon = (t_localized_pokemon*) request->structure;
            return amount_of_bytes_of_localized(localized_pokemon);
        }
        case GET_POKEMON:{
            t_get_pokemon* get_pokemon = (t_get_pokemon*) request->structure;
            return amount_of_bytes_of_get(get_pokemon);
        }
        case APPEARED_POKEMON:{
            t_appeared_pokemon* appeared_pokemon = (t_appeared_pokemon*) request->structure;
            return amount_of_bytes_of_appeared(appeared_pokemon);
        }
        case CATCH_POKEMON:{
            t_catch_pokemon* catch_pokemon = (t_catch_pokemon*) request->structure;
            return amount_of_bytes_of_catch(catch_pokemon);
        }
        case CAUGHT_POKEMON:{
            return sizeof(uint32_t); //caught status 0-1
        }
        case SUBSCRIBE_ME:{
            t_subscribe_me* subscribe_me = (t_subscribe_me*) request->structure;
            return amount_of_bytes_of_subscribe_me(subscribe_me);
        }

    }
    return size;
}

