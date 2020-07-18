#include <stdio.h>
#include <zconf.h>
#include "commons/string.h"
#include "commons/config.h"
#include <string.h>
#include <stdlib.h>

void broker_config_updater(t_config* config);
void team_config_updater(t_config* config);
void gamecard_config_updater(t_config* config);
void gameboy_config_updater(t_config* config);

char** config_arguments;

int main(int arguments_amount, char* arguments[]){

    if (arguments_amount != 2) {
        printf("Se ingresó una cantidad de argumentos incorrecta!\n");
        return 0;
    }

    config_arguments = arguments;
    char* process_name = config_arguments[1];
    char* executable_path = getcwd(NULL, 0);

    char* reversed_path_for_split = string_reverse(executable_path);
    char** reversed_path_splitted = string_n_split(reversed_path_for_split, 3, "/");
    char* config_absolute_path = string_reverse(reversed_path_splitted[2]);

    char* config_name = string_duplicate(process_name);
    string_append(&config_name, ".config");

    string_capitalized(process_name);

    string_append(&config_absolute_path, "/");
    string_append(&config_absolute_path, process_name);
    string_append(&config_absolute_path, "/");

    string_append(&config_absolute_path, config_name);

    char* config_description = string_from_format("Este es el archivo config del proceso: %s", config_name);

    printf("%s\n", config_description);

    t_config* config = config_create(config_absolute_path);

    if (string_equals_ignore_case(process_name, "Broker")) {
        broker_config_updater(config);
    }

    if (string_equals_ignore_case(process_name, "Team")) {
        team_config_updater(config);
    }

    if (string_equals_ignore_case(process_name, "Gamecard")) {
        gamecard_config_updater(config);
    }

    if (string_equals_ignore_case(process_name, "Gameboy")) {
        gameboy_config_updater(config);
    }
}

void broker_config_updater(t_config* config){

    char* scanned_line = string_new();

    printf("Ingrese de la siguiente manera los siguientes comandos: Numero-ValorDeseado\n");
    printf("Ingrese 1 y su nuevo valor para cambiar el tamaño de la memoria\n");
    printf("Ingrese 2 y su nuevo valor para cambiar el tamaño minimo de particion\n");
    printf("Ingrese 3 y su nuevo valor para cambiar el algoritmo de memoria\n");
    printf("Ingrese 4 y su nuevo valor para cambiar el algoritmo de reemplazo\n");
    printf("Ingrese 5 y su nuevo valor para cambiar el algoritmo de particion libre\n");
    printf("Ingrese 6 y su nuevo valor para cambiar la ip\n");
    printf("Ingrese 7 y su nuevo valor para cambiar el puerto\n");
    printf("Ingrese 8 y su nuevo valor para cambiar la frecuencia de compactacion\n");
    printf("Ingrese 9 y su nuevo valor para cambiar el ack timeout\n");
    printf("Ingrese 10 y su nuevo valor para cambiar el path del log\n");
    printf("Ingrese 0 para terminar.\n");

    scanf("%s", scanned_line);

    char** scanned_line_splitted = string_split(scanned_line, "-");

    int number = atoi(scanned_line_splitted[0]);
    char* value = scanned_line_splitted[1];

    while (number != 0) {

        switch (number) {
            case 1 :
                config_set_value(config, "TAMANO_MEMORIA", value);
                break;
            case 2 :
                config_set_value(config, "TAMANO_MINIMO_PARTICION", value);
                break;
            case 3 :
                config_set_value(config, "ALGORITMO_MEMORIA", value);
                break;
            case 4 :
                config_set_value(config, "ALGORITMO_REEMPLAZO", value);
                break;
            case 5 :
                config_set_value(config, "ALGORITMO_PARTICION_LIBRE", value);
                break;
            case 6 :
                config_set_value(config, "IP_BROKER", value);
                break;
            case 7 :
                config_set_value(config, "PUERTO_BROKER", value);
                break;
            case 8 :
                config_set_value(config, "FRECUENCIA_COMPACTACION", value);
                break;
            case 9 :
                config_set_value(config, "ACK_TIMEOUT", value);
                break;
            case 10 :
                config_set_value(config, "LOG_FILE", value);
                break;
            default :
                printf("No se ingreso una operacion valida\n");
                break;
        }

        scanf("%s", scanned_line);

        char** scanned_line_split = string_split(scanned_line, "-");

        number = atoi(scanned_line_split[0]);

        config_save(config);
    }
}

void team_config_updater(t_config* config){
    char *scanned_line = string_new();

    printf("Ingrese de la siguiente manera los siguientes comandos: Numero-ValorDeseado\n");
    printf("Ingrese 1 y su nuevo valor para cambiar valores de posiciones de los entrenadores\n");
    printf("Ingrese 2 y su nuevo valor para cambiar los pokemones que tienen los entrenadores\n");
    printf("Ingrese 3 y su nuevo valor para cambiar los objetivos de los entrenadores\n");
    printf("Ingrese 4 y su nuevo valor para cambiar el tiempo de reconexion\n");
    printf("Ingrese 5 y su nuevo valor para cambiar el retardo de ciclo de cpu\n");
    printf("Ingrese 6 y su nuevo valor para cambiar el algoritmo de planificacion\n");
    printf("Ingrese 7 y su nuevo valor para cambiar el quantum\n");
    printf("Ingrese 8 y su nuevo valor para cambiar la estimacion inicial\n");
    printf("Ingrese 9 y su nuevo valor para cambiar el alpha\n");
    printf("Ingrese 10 y su nuevo valor para cambiar la ip del broker\n");
    printf("Ingrese 11 y su nuevo valor para cambiar el puerto del broker\n");
    printf("Ingrese 12 y su nuevo valor para cambiar el puerto\n");
    printf("Ingrese 13 y su nuevo valor para cambiar el ack timeout\n");
    printf("Ingrese 14 y su nuevo valor para cambiar el path del log\n");
    printf("Ingrese 0 para terminar.\n");

    scanf("%s", scanned_line);

    char** scanned_line_splitted = string_split(scanned_line, "-");

    int number = atoi(scanned_line_splitted[0]);
    char* value = scanned_line_splitted[1];

    while (number != 0) {

        switch (number) {
            case 1 :
                config_set_value(config, "POSICIONES_ENTRENADORES", value);
                break;
            case 2 :
                config_set_value(config, "POKEMON_ENTRENADORES", value);
                break;
            case 3 :
                config_set_value(config, "OBJETIVOS_ENTRENADORES", value);
                break;
            case 4 :
                config_set_value(config, "TIEMPO_RECONEXION", value);
                break;
            case 5 :
                config_set_value(config, "RETARDO_CICLO_CPU", value);
                break;
            case 6 :
                config_set_value(config, "ALGORITMO_PLANIFICACION", value);
                break;
            case 7 :
                config_set_value(config, "QUANTUM", value);
                break;
            case 8 :
                config_set_value(config, "ESTIMACION_INICIAL", value);
                break;
            case 9 :
                config_set_value(config, "ALPHA", value);
                break;
            case 10 :
                config_set_value(config, "IP_BROKER", value);
                break;
            case 11 :
                config_set_value(config, "PUERTO_BROKER", value);
                break;
            case 12 :
                config_set_value(config, "PUERTO", value);
                break;
            case 13 :
                config_set_value(config, "ACK_TIMEOUT", value);
                break;
            case 14 :
                config_set_value(config, "LOG_FILE", value);
                break;
            default :
                printf("No se ingreso una operacion valida\n");
                break;
        }

        scanf("%s", scanned_line);

        char** scanned_line_split = string_split(scanned_line, "-");

        number = atoi(scanned_line_split[0]);

        config_save(config);
    }
}

void gamecard_config_updater(t_config* config){
    char *scanned_line = string_new();

    printf("Ingrese de la siguiente manera los siguientes comandos: Numero-ValorDeseado\n");
    printf("Ingrese 1 y su nuevo valor para cambiar el tiempo de reintento de conexion con el broker\n");
    printf("Ingrese 2 y su nuevo valor para cambiar el tiempo de retardo de operacion\n");
    printf("Ingrese 3 y su nuevo valor para cambiar el tiempo de reintento de operacion\n");
    printf("Ingrese 4 y su nuevo valor para cambiar la ip del broker\n");
    printf("Ingrese 5 y su nuevo valor para cambiar el puerto del broker\n");
    printf("Ingrese 6 y su nuevo valor para cambiar el puerto\n");
    printf("Ingrese 7 y su nuevo valor para cambiar punto de montaje de tallgrass\n");
    printf("Ingrese 8 y su nuevo valor para cambiar el ack timeout\n");
    printf("Ingrese 0 para terminar.\n");

    scanf("%s", scanned_line);

    char** scanned_line_splitted = string_split(scanned_line, "-");

    int number = atoi(scanned_line_splitted[0]);
    char* value = scanned_line_splitted[1];

    while (number != 0) {

        switch (number) {
            case 1 :
                config_set_value(config, "TIEMPO_DE_REINTENTO_CONEXION", value);
                break;
            case 2 :
                config_set_value(config, "TIEMPO_RETARDO_OPERACION", value);
                break;
            case 3 :
                config_set_value(config, "TIEMPO_DE_REINTENTO_OPERACION", value);
                break;
            case 4 :
                config_set_value(config, "IP_BROKER", value);
                break;
            case 5 :
                config_set_value(config, "PUERTO_BROKER", value);
                break;
            case 6 :
                config_set_value(config, "PUERTO", value);
                break;
            case 7 :
                config_set_value(config, "PUNTO_MONTAJE_TALLGRASS", value);
                break;
            case 8 :
                config_set_value(config, "ACK_TIMEOUT", value);
                break;
            default :
                printf("No se ingreso una operacion valida\n");
                break;
        }

        scanf("%s", scanned_line);

        char** scanned_line_split = string_split(scanned_line, "-");

        number = atoi(scanned_line_split[0]);

        config_save(config);
    }
}

void gameboy_config_updater(t_config* config){
    char *scanned_line = string_new();

    printf("Ingrese de la siguiente manera los siguientes comandos: Numero-ValorDeseado\n");
    printf("Ingrese 1 y su nuevo valor para cambiar la ip del broker\n");
    printf("Ingrese 2 y su nuevo valor para cambiar la ip del team\n");
    printf("Ingrese 3 y su nuevo valor para cambiar la ip del gamecard\n");
    printf("Ingrese 4 y su nuevo valor para cambiar el puerto del broker\n");
    printf("Ingrese 5 y su nuevo valor para cambiar el puerto del team\n");
    printf("Ingrese 6 y su nuevo valor para cambiar el puerto del gamecard\n");
    printf("Ingrese 7 y su nuevo valor para cambiar el ack timeout\n");
    printf("Ingrese 8 y su nuevo valor para cambiar el log file\n");
    printf("Ingrese 0 para terminar.\n");

    scanf("%s", scanned_line);

    char** scanned_line_splitted = string_split(scanned_line, "-");

    int number = atoi(scanned_line_splitted[0]);
    char* value = scanned_line_splitted[1];

    while (number != 0) {

        switch (number) {
            case 1 :
                config_set_value(config, "IP_BROKER", value);
                break;
            case 2 :
                config_set_value(config, "IP_TEAM", value);
                break;
            case 3 :
                config_set_value(config, "IP_GAMECARD", value);
                break;
            case 4 :
                config_set_value(config, "PUERTO_BROKER", value);
                break;
            case 5 :
                config_set_value(config, "PUERTO_TEAM", value);
                break;
            case 6 :
                config_set_value(config, "PUERTO_GAMECARD", value);
                break;
            case 7 :
                config_set_value(config, "ACK_TIMEOUT", value);
                break;
            case 8 :
                config_set_value(config, "LOG_FILE", value);
                break;
            default :
                printf("No se ingreso una operacion valida\n");
                break;
        }

        scanf("%s", scanned_line);

        char** scanned_line_split = string_split(scanned_line, "-");

        number = atoi(scanned_line_split[0]);

        config_save(config);
    }
}