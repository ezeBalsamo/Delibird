#include <commons/collections/list.h>
#include <commons/string.h>
#include "../include/serializable_objects.h"
#include "../include/common_structures.h"
#include "../include/operation_serialization.h"
#include "../include/operation_deserialization.h"
#include <stdlib.h>
#include <t_list_extension.h>
#include <queue_code_name_associations.h>

t_list* all_serializables;

void initialize_and_load_serializable_new_pokemon(){
    t_serializable_object* serializable_object = malloc(sizeof(t_serializable_object));
    serializable_object -> code = NEW_POKEMON;
    serializable_object -> serialize_function = serialize_new_pokemon;
    serializable_object -> deserialize_function = deserialize_new_pokemon;

    list_add(all_serializables, (void*) serializable_object);
}

void initialize_and_load_serializable_appeared_pokemon(){
    t_serializable_object* serializable_object = malloc(sizeof(t_serializable_object));
    serializable_object -> code = APPEARED_POKEMON;
    serializable_object -> serialize_function = serialize_appeared_pokemon;
    serializable_object -> deserialize_function = deserialize_appeared_pokemon;

    list_add(all_serializables, (void*) serializable_object);
}

void initialize_and_load_serializable_get_pokemon(){
    t_serializable_object* serializable_object = malloc(sizeof(t_serializable_object));
    serializable_object -> code = GET_POKEMON;
    serializable_object -> serialize_function = serialize_get_pokemon;
    serializable_object -> deserialize_function = deserialize_get_pokemon;

    list_add(all_serializables, (void*) serializable_object);
}

void initialize_and_load_serializable_localized_pokemon(){
    t_serializable_object* serializable_object = malloc(sizeof(t_serializable_object));
    serializable_object -> code = LOCALIZED_POKEMON;
    serializable_object -> serialize_function = serialize_localized_pokemon;
    serializable_object -> deserialize_function = deserialize_localized_pokemon;

    list_add(all_serializables, (void*) serializable_object);
}

void initialize_and_load_serializable_catch_pokemon(){
    t_serializable_object* serializable_object = malloc(sizeof(t_serializable_object));
    serializable_object -> code = CATCH_POKEMON;
    serializable_object -> serialize_function = serialize_catch_pokemon;
    serializable_object -> deserialize_function = deserialize_catch_pokemon;

    list_add(all_serializables, (void*) serializable_object);
}

void initialize_and_load_serializable_caught_pokemon(){
    t_serializable_object* serializable_object = malloc(sizeof(t_serializable_object));
    serializable_object -> code = CAUGHT_POKEMON;
    serializable_object -> serialize_function = serialize_caught_pokemon;
    serializable_object -> deserialize_function = deserialize_caught_pokemon;

    list_add(all_serializables, (void*) serializable_object);
}

void initialize_and_load_serializable_subscribe_me(){
    t_serializable_object* serializable_object = malloc(sizeof(t_serializable_object));
    serializable_object -> code = SUBSCRIBE_ME;
    serializable_object -> serialize_function = serialize_subscribe_me;
    serializable_object -> deserialize_function = deserialize_subscribe_me;

    list_add(all_serializables, (void*) serializable_object);
}

void initialize_and_load_serializable_identified_message(){
    t_serializable_object* serializable_object = malloc(sizeof(t_serializable_object));
    serializable_object -> code = IDENTIFIED_MESSAGE;
    serializable_object -> serialize_function = serialize_identified_message;
    serializable_object -> deserialize_function = deserialize_identified_message;

    list_add(all_serializables, (void*) serializable_object);
}

void initialize_serializable_objects(){

    initialize_queue_code_name_associations();

    all_serializables = list_create();
    initialize_and_load_serializable_new_pokemon();
    initialize_and_load_serializable_appeared_pokemon();
    initialize_and_load_serializable_get_pokemon();
    initialize_and_load_serializable_localized_pokemon();
    initialize_and_load_serializable_catch_pokemon();
    initialize_and_load_serializable_caught_pokemon();
    initialize_and_load_serializable_subscribe_me();
    initialize_and_load_serializable_identified_message();
}

t_serializable_object* serializable_new_pokemon(){
    return serializable_object_with_code(NEW_POKEMON);
}

t_serializable_object* serializable_appeared_pokemon(){
    return serializable_object_with_code(APPEARED_POKEMON);
}

t_serializable_object* serializable_catch_pokemon(){
    return serializable_object_with_code(CATCH_POKEMON);
}

t_serializable_object* serializable_caught_pokemon(){
    return serializable_object_with_code(CAUGHT_POKEMON);
}

t_serializable_object* serializable_get_pokemon(){
    return serializable_object_with_code(GET_POKEMON);
}

t_serializable_object* serializable_localized_pokemon(){
    return serializable_object_with_code(LOCALIZED_POKEMON);
}

t_serializable_object* serializable_subscribe_me(){
    return serializable_object_with_code(SUBSCRIBE_ME);
}

t_serializable_object* serializable_identified_me(){
    return serializable_object_with_code(IDENTIFIED_MESSAGE);
}

t_serializable_object* serializable_object_with_code(uint32_t operation_code){

    bool _is_for(void* serializable_object){
        return ((t_serializable_object*) serializable_object) -> code == operation_code;
    }
    t_serializable_object* serializable_object_found = list_find(all_serializables, _is_for);

    return serializable_object_found;
}

void free_serializable_objects(){
    free_queue_code_name_associations();
    list_destroy_and_destroy_elements(all_serializables, free);
}