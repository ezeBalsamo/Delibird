#include "../include/entry_point_validator.h"

t_dictionary* process_dictionary;
t_dictionary* operations_dictionary;

void initialize_structs(){
    initialize_process_dictionary();
    initialize_operations_dictionary();
}

void initialize_process_dictionary(){

    process_dictionary = dictionary_create();
    dictionary_put(process_dictionary, "TEAM", TEAM);
    dictionary_put(process_dictionary, "BROKER", BROKER);
    dictionary_put(process_dictionary, "GAMECARD", GAMECARD);
    dictionary_put(process_dictionary, "SUSCRIPTOR", SUSCRIPTOR);
}

void initialize_operations_dictionary(){

    operations_dictionary = dictionary_create();
    dictionary_put(operations_dictionary, "APPEARED_POKEMON", APPEARED_POKEMON);
    dictionary_put(operations_dictionary, "CATCH_POKEMON", CATCH_POKEMON);
    dictionary_put(operations_dictionary, "CAUGHT_POKEMON", CAUGHT_POKEMON);
    dictionary_put(operations_dictionary, "GET_POKEMON", GET_POKEMON);
    dictionary_put(operations_dictionary, "NEW_POKEMON", NEW_POKEMON);
}

int valid_process_from(char* process) {

    string_to_upper(process);
    void *process_code = dictionary_get(process_dictionary, process);

    if (process_code == NULL) {
        printf("No se ingresó un proceso válido.\n");
        exit(EXIT_FAILURE);
    }
    return (int*) process_code;
}


int valid_operation_from(char* operation){
    string_to_upper(operation);
    void* operation_code = dictionary_get(operations_dictionary, operation);

    if(operation_code == NULL){
        printf("No se ingresó una operación válida.\n");
        exit(EXIT_FAILURE);
    }
    return (int*) operation_code;
}

void assert_valid_operation(int process_code, int operation_code) {

    switch (process_code) {

        case TEAM: {
                printf("Se envia el mensaje al proceso Team.\n");

            if (operation_code != APPEARED_POKEMON) {
                printf("Operación no aceptada por el proceso Team.\n");
                exit(EXIT_FAILURE);
            }
            break;
        }

        case BROKER: {

            printf("Se envia el mensaje al proceso Broker.\n");

            if (operation_code != NEW_POKEMON && operation_code != APPEARED_POKEMON &&
                operation_code != CATCH_POKEMON && operation_code != CAUGHT_POKEMON &&
                operation_code != GET_POKEMON) {
                printf("Operación no aceptada por el proceso Broker.\n");
                exit(EXIT_FAILURE);
            }
            break;
        }

        case GAMECARD: {

            printf("Se envia el mensaje al proceso GameCard.\n");

            if (operation_code != NEW_POKEMON && operation_code != CATCH_POKEMON &&
                operation_code != GET_POKEMON) {
                printf("Operación no aceptada por el proceso GameCard.\n");
                exit(EXIT_FAILURE);
            }
            break;
        }

        case SUSCRIPTOR: {

            printf("Se envia el mensaje al proceso Suscriptor.\n");

            break;
        }//TODO

        default: {
            printf("No se ingreso un proceso válido.\n");
            exit(EXIT_FAILURE);
            break;
        }
    }
}
void assert_valid_operation_arguments(int process_code, int operation_code, int arguments_amount, char* arguments) {

    switch (process_code) { //No se como hacer para validar los parametros que te mandan...
                            // Ver despues (por eso le paso los argumentos tambien!).
        case TEAM: {

            if (operation_code == APPEARED_POKEMON) {
                printf("Operacion APPEARED_POKEMON recibida.\n");
                if (arguments_amount != 6) {
                    printf("Cantidad de argumentos incorrecta.\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                printf("No se ingreso una operación válida para el proceso Team.\n");
                exit(EXIT_FAILURE);
            }
            break;
        }

        case BROKER: {

            switch (operation_code) {

                case NEW_POKEMON: {
                    printf("Operacion NEW_POKEMON recibida.\n");
                    if (arguments_amount != 7) {
                        printf("Cantidad de argumentos incorrecta.\n");
                        exit(EXIT_FAILURE);
                    }
                    break;
                }

                case APPEARED_POKEMON: {
                    printf("Operacion APPEARED_POKEMON recibida.\n");
                    if (arguments_amount != 7) {
                        printf("Cantidad de argumentos incorrecta.\n");
                        exit(EXIT_FAILURE);
                    }
                    break;
                }

                case CATCH_POKEMON: {
                    printf("Operacion CATCH_POKEMON recibida.\n");
                    if (arguments_amount != 6) {
                        printf("Cantidad de argumentos incorrecta.\n");
                        exit(EXIT_FAILURE);
                    }
                    break;
                }

                case CAUGHT_POKEMON: {
                    printf("Operacion CAUGHT_POKEMON recibida.\n");
                    if (arguments_amount != 5) {
                        printf("Cantidad de argumentos incorrecta.\n");
                        exit(EXIT_FAILURE);
                    }
                    break;
                }

                case GET_POKEMON: {
                    printf("Operacion GET_POKEMON recibida.\n");
                    if (arguments_amount != 4) {
                        printf("Cantidad de argumentos incorrecta.\n");
                        exit(EXIT_FAILURE);
                    }
                    break;
                }

                default: {
                    printf("No se ingreso una operación válida para el proceso Broker.\n");
                    exit(EXIT_FAILURE);
                    break;
                }
            }
            break;
        }

        case GAMECARD: {

            switch (operation_code) {

                case NEW_POKEMON: {
                    printf("Operacion NEW_POKEMON recibida.\n");
                    if (arguments_amount != 7) {
                        printf("Cantidad de argumentos incorrecta.\n");
                        exit(EXIT_FAILURE);
                    }
                    break;
                }

                case CATCH_POKEMON: {
                    printf("Operacion CATCH_POKEMON recibida.\n");
                    if (arguments_amount != 6) {
                        printf("Cantidad de argumentos incorrecta.\n");
                        exit(EXIT_FAILURE);
                    }
                    break;
                }

                case GET_POKEMON: {
                    printf("Operacion GET_POKEMON recibida.\n");
                    if (arguments_amount != 4) {
                        printf("Cantidad de argumentos incorrecta.\n");
                        exit(EXIT_FAILURE);
                    }
                    break;
                }

                default: {
                    printf("No se ingreso una operación válida para el proceso GameCard.\n");
                    exit(EXIT_FAILURE);
                    break;
                }
            }
            break;
        }
        case SUSCRIPTOR: { //TODO

            if (arguments_amount != 4) {
                printf("Cantidad de argumentos incorrecta.");
                exit(EXIT_FAILURE);
            }
        break;
        }
    }
}
