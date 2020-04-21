
#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/dictionary.h>
#include <commons/string.h>

typedef enum Process {
    NULL_PROCESS, TEAM, BROKER, GAMECARD, SUSCRIPTOR
} t_process;

typedef enum Operation {
    NULL_OPERATION, APPEARED_POKEMON, CATCH_POKEMON, CAUGHT_POKEMON,
    GET_POKEMON, NEW_POKEMON
} t_operation;

void initialize_structs();

int valid_process_from(char*);

int valid_operation_from(char* );

void assert_valid_operation(int , int );

void assert_valid_operation_arguments(int , int , int , char* );



