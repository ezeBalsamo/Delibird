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

void print_line(char* key, char* value){
    char* line = string_from_format("%s=%s", key, value);
    printf("%s\n", line);
    free(line);
}

char* config_absolute_path_for(char** arguments){

    config_arguments = arguments;
    char* process_name = config_arguments[1];
    char* executable_path = getcwd(NULL, 0);

    char* reversed_path_for_split = string_reverse(executable_path);
    char** reversed_path_splitted = string_n_split(reversed_path_for_split, 3, "/");
    char* project_path = string_reverse(reversed_path_splitted[2]);

    char* config_name = string_duplicate(process_name);
    string_append(&config_name, ".config");

    string_capitalized(process_name);

    printf("Este es el archivo config del proceso: %s\n", config_name);
    char* config_absolute_path = string_from_format("%s/%s/%s", project_path, process_name, config_name);

    free(executable_path);
    free(reversed_path_for_split);
    string_iterate_lines(reversed_path_splitted, (void (*)(char *)) free);
    free(reversed_path_splitted);
    free(project_path);
    free(config_name);

    return config_absolute_path;
}

void team_options_print(){
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
}

void team_switch_options_for(int number, char* value, t_config* config){

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
}

void gamecard_options_print(){
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
}

void gamecard_switch_options_for(int number, char* value, t_config* config){
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
}

void broker_options_print(){
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
}

void broker_switch_options_for(int number, char* value, t_config* config) {
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
}

void gameboy_options_print(){
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
}

void gameboy_switch_options_for(int number, char* value, t_config* config){
    switch(number){
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
}

int main(int arguments_amount, char* arguments[]){

    if (arguments_amount != 2) {
        printf("Se ingresó una cantidad de argumentos incorrecta!\n");
        return 0;
    }

    char* config_absolute_path = config_absolute_path_for(arguments);
    t_config* config = config_create(config_absolute_path);
    free(config_absolute_path);

    dictionary_iterator(config -> properties, (void (*)(char *, void *)) print_line);

    char* process_name = config_arguments[1];

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

void basic_config_updater_using(t_config* config, void (*options_printer) (), void (*options_updater) (int, char*, t_config*)){

    char scanned_line[20];

    options_printer();

    scanf("%s", scanned_line);

    if(atoi(scanned_line) == 0){
        config_destroy(config);
    }else{
        char** scanned_line_splitted = string_split(scanned_line, "-");

        int number = atoi(scanned_line_splitted[0]);
        char* value = scanned_line_splitted[1];

        while (number != 0) {

            options_updater(number, value, config);
            string_iterate_lines(scanned_line_splitted, (void (*)(char *)) free);
            free(scanned_line_splitted);
            config_save(config);
            printf("Config actualizado. Ingrese nuevo valor:\n");

            scanf("%s", scanned_line);

            if(atoi(scanned_line) == 0){
                config_destroy(config);
                number = 0;
            }else{
                char** next_scanned_line_spplitted = string_split(scanned_line, "-");

                number = atoi(next_scanned_line_spplitted[0]);
                value = next_scanned_line_spplitted[1];
            }
        }
    }
}

void broker_config_updater(t_config* config){
    basic_config_updater_using(config, broker_options_print, (void (*)(int, char *, t_config *)) broker_switch_options_for);
}

void team_config_updater(t_config* config){
    basic_config_updater_using(config, team_options_print, (void (*)(int, char *, t_config *)) team_switch_options_for);
}

void gamecard_config_updater(t_config* config){
    basic_config_updater_using(config, gamecard_options_print, (void (*)(int, char *, t_config *)) gamecard_switch_options_for);
}

void gameboy_config_updater(t_config* config){
    basic_config_updater_using(config, gameboy_options_print, gameboy_switch_options_for);
}