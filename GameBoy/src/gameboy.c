#include "../include/entry_point_validator.h"
#include <stdio.h>
#include <stdlib.h>

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