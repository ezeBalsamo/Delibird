#ifndef COMMON_STRUCTURES_H
#define COMMON_STRUCTURES_H

#include <stdint.h>
#include <commons/collections/list.h>
#include <stddef.h>
#include <semaphore.h>
#include "serialization_interface.h"

#define ever (;;)

typedef struct Appeared_pokemon{
    char* pokemon_name;
    uint32_t pos_x;
    uint32_t pos_y;
}t_appeared_pokemon;

typedef struct New_pokemon{
    char* pokemon_name;
    uint32_t pos_x;
    uint32_t pos_y;
    uint32_t quantity;
}t_new_pokemon;

typedef struct Catch_pokemon{
    char* pokemon_name;
    uint32_t pos_x;
    uint32_t pos_y;
}t_catch_pokemon;

typedef struct Caught_pokemon{
    uint32_t caught_status;
}t_caught_pokemon;

typedef struct Get_pokemon{
    char* pokemon_name;
}t_get_pokemon;

typedef struct Localized_pokemon{
    char* pokemon_name;
    uint32_t quantity;
    t_list* positions;
}t_localized_pokemon;

typedef struct Subscribe_me{
    uint32_t operation_queue;
    char* process_description;
}t_subscribe_me;

typedef struct Identified_message{
    uint32_t message_id;
    t_request* request;
}t_identified_message;

enum Operation {
    NEW_POKEMON, APPEARED_POKEMON,
    GET_POKEMON, LOCALIZED_POKEMON,
    CATCH_POKEMON, CAUGHT_POKEMON,
    SUBSCRIBE_ME, IDENTIFIED_MESSAGE
};

uint32_t internal_operation_in(t_identified_message* identified_message);
uint32_t internal_operation_in_correlative(t_identified_message* correlative_identified_message);
void* internal_object_in(t_identified_message* identified_message);
void* internal_object_in_correlative(t_identified_message* correlative_identified_message);
void* internal_request_in_correlative(t_identified_message* correlative_identified_message);

void initialize_signal_handler();
void handle_signal(int signal_number, void (*handler_function) ());

void safe_sem_initialize(sem_t* semaphore);
void safe_sem_destroy(sem_t* semaphore);

void* safe_malloc(size_t size);

char* process_description_for(char* process_name, t_list* strings_to_hash);
uint64_t current_time_in_milliseconds();
void assert_only_one_in(t_list* self);

void free_system_debugging_thread_alive_for(int seconds);

void sleep_for(int reconnection_time_in_seconds);
int minimum_integer_between(int number, int another_number);

void free_identified_message(t_identified_message* identified_message);
void free_localized_pokemon(t_localized_pokemon* localized_pokemon);
void free_subscribe_me(t_subscribe_me* subscribe_me);
void free_char_array(char**);

#endif //COMMON_STRUCTURES_H