#include "../include/entry_point_validator.h"

int main(int arguments_amount, char* arguments[]){

    if(arguments_amount < 3){
        printf("No se ingresaron los argumentos suficientes.\n");
        //TODO reemplazar printf por logs
        exit(EXIT_FAILURE);
    }

    initialize_structs();

    int process_code = valid_process_from(arguments[1]);  //OJO EL ./gameboy es el lugar 0.

    int operation_code = valid_operation_from(arguments[2]);

    assert_valid_operation(process_code, operation_code);

    assert_valid_operation_arguments(process_code, operation_code, arguments_amount, arguments);
}
/*
        case TEAM:{

            request = armar_request_team(operacion, argumentos);
            ip = get_from_config();
            puerto = get_from_config();
            break;
        };

        case BROKER:{

            request = armar_request_broker(operacion, argumentos);
            ip = get_from_config();
            puerto = get_from_config();
            break;
        };

        case GAMECARD:{

            request = armar_request_gamecard(operacion, argumentos);
            ip = get_from_config();
            puerto = get_from_config();
        };

        default:
            printf("No se ingresó un proceso correcto del sistema.");
            return 0;
    }

    int socket_fd = connect_to(ip, puerto);
    send_structure(request, socket_fd);
}
*/
