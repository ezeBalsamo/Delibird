#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/matrix.h"

void print_matrix_elem(void* matrix_element){
    printf("%10d", *((int*) matrix_element));
}

void move_elements_to_upper_row_according_to(t_matrix* self, uint32_t amount_of_elements){

    int row_index = amount_of_elements;

    for(int amount_of_moves_between_rows = 0; amount_of_moves_between_rows < amount_of_elements; amount_of_moves_between_rows++){
        void* origin = (self -> data)[row_index][amount_of_moves_between_rows];
        (self -> data)[row_index - 1][self -> amount_of_columns - amount_of_elements + amount_of_moves_between_rows] = origin;
    }
}

void reorder_elements_in_the_same_row_according_to(t_matrix* self, uint32_t amount_of_elements_sent_to_upper_row){

    int row_index = amount_of_elements_sent_to_upper_row;
    uint32_t amount_of_elements_to_move_in_the_same_row = self -> amount_of_columns - 1 - amount_of_elements_sent_to_upper_row;

    for(int new_column_element_index = 0; new_column_element_index < amount_of_elements_to_move_in_the_same_row; new_column_element_index++){
        void* origin = (self -> data)[row_index][new_column_element_index + amount_of_elements_sent_to_upper_row];
        (self -> data)[row_index][new_column_element_index] = origin;
    }
}

void reorder(t_matrix* self){
    for(int row_index = 1; row_index < self -> amount_of_rows; row_index++){

        //Cuando se agrega una columna, se desplazan n - 1 elementos de la fila n a la fila n - 1
        //Esto implica que la cantidad de elementos a subir concuerda con el índice de la fila de la matriz
        //Ej.: De la fila 2 (índice 1), se desplaza un elemento a la fila superior

        uint32_t amount_of_elements_to_send_to_upper_row = row_index;
        move_elements_to_upper_row_according_to(self, amount_of_elements_to_send_to_upper_row);

        //Cuando se agrega una columna, calculo la cantidad de elementos de la misma fila a reordenar como
        //cantidad de elementos = cantidad de columnas - 1 - cantidad de elementos que subieron una fila
        reorder_elements_in_the_same_row_according_to(self, amount_of_elements_to_send_to_upper_row);

        //Cuando me encuentro en la última fila, debo limpiar los espacios previamente ocupados ya que ahora
        //se liberaron por los desplazamientos
        bool is_last_row = self -> amount_of_rows - amount_of_elements_to_send_to_upper_row == 1;
        if (is_last_row){
            for(int column_indexes_to_clean = 1; column_indexes_to_clean < self -> amount_of_columns; column_indexes_to_clean++){
                (self -> data)[self -> amount_of_rows - 1][column_indexes_to_clean] = NULL;
            }
        }
    }
}

void add_row(t_matrix* self){
    self -> amount_of_rows++;
    self -> data = realloc(self -> data, self -> amount_of_rows * sizeof(void**)); // Resigno espacio para una fila más
    (self -> data)[self -> amount_of_rows - 1] = calloc(self -> amount_of_columns, sizeof(void*)); // Reasigno espacio para las columnas de esa fila
}

void add_column(t_matrix* self){
    self -> amount_of_columns++;

    //A cada fila, le reasigno espacio considerando que ahora tienen una columna más
    for(int row_index = 0; row_index < self -> amount_of_rows; row_index++){
        (self -> data)[row_index] = realloc((self -> data)[row_index], self -> amount_of_columns * sizeof(void*));
    }

    if(self -> must_reorder_after_insert){
        reorder(self);
    }
}

void expand(t_matrix* self, uint32_t row_index, uint32_t column_index){

    if(self -> must_be_square){
        uint32_t size = row_index > column_index ? row_index : column_index;

        while (self -> amount_of_rows < size){
            add_row(self);
        }
        while (self -> amount_of_columns < size){
            add_column(self);
        }
    }
    else{
        while(self -> amount_of_rows < row_index){
            add_row(self);
        }

        while (self -> amount_of_columns < column_index){
            add_column(self);
        }
    }
}

void consider_expanding(t_matrix* self, uint32_t row_index, uint32_t column_index){

    if (row_index > self -> amount_of_rows || column_index > self -> amount_of_columns){
        if (self -> is_expandable){
            expand(self, row_index, column_index);
        }
        else{
            printf("No se puede ampliar una matriz definida como no ampliable\n");
            exit(EXIT_FAILURE);
        }
    }
}

t_matrix* matrix_create(uint32_t amount_of_rows, uint32_t amount_of_columns, bool should_be_expandable, bool should_be_squared, bool should_reorder_after_insert){

    if(amount_of_rows <= 0 || amount_of_columns <= 0){
        printf("Las filas y columnas deben ser mayores a cero\n");
        exit(EXIT_FAILURE);
    }

    if(should_be_squared && amount_of_rows != amount_of_columns){
        printf("Una matriz cuadrada tiene igual cantidad de filas y columnas\n");
        exit(EXIT_FAILURE);
    }

    t_matrix* matrix = malloc(sizeof(t_matrix));
    matrix -> amount_of_rows = amount_of_rows;
    matrix -> amount_of_columns = amount_of_columns;

    matrix -> data = malloc(amount_of_rows * sizeof(void**));
    for(int row_index = 0; row_index < matrix -> amount_of_rows; row_index++){
        (matrix -> data)[row_index] = calloc(amount_of_columns, sizeof(void*));
    }

    matrix -> must_be_square = should_be_squared;
    matrix -> is_expandable = should_be_expandable;
    matrix -> must_reorder_after_insert = should_reorder_after_insert;
    return matrix;
}

void assert_strictly_positive_indexes(uint32_t row_index, uint32_t column_index){

    if(row_index <= 0 || column_index <= 0){
        printf("Los índices de filas y columnas deben ser mayores a cero\n");
        exit(EXIT_FAILURE);
    }
}

void insert_matrix_element_at(t_matrix* self, void* element, uint32_t row_index, uint32_t column_index){

    assert_strictly_positive_indexes(row_index, column_index);

    consider_expanding(self, row_index, column_index);
    (self -> data)[row_index - 1][column_index - 1] = element;
}

void* matrix_element_at(t_matrix* self, uint32_t row_index, uint32_t column_index){

    assert_strictly_positive_indexes(row_index, column_index);

    if(self -> amount_of_rows < row_index || self -> amount_of_columns < column_index){
        printf("Las posiciones elegidas exceden las dimensiones de la matriz\n");
        exit(EXIT_FAILURE);
    }
    return (self -> data)[row_index - 1][column_index - 1];
}

void matrix_iterate_doing(t_matrix* self, void (*closure) (void*)){

    for(uint32_t row_index = 1; row_index <= self -> amount_of_rows; row_index++){
        for(uint32_t column_index = 1; column_index <= self -> amount_of_columns; column_index++){
            void* matrix_element = matrix_element_at(self, row_index, column_index);
            (*closure) (matrix_element);
        }
    }
}

void print_matrix(t_matrix* self, void (*matrix_element_printer) (void*)){
    for(uint32_t row_number = 1; row_number <= self -> amount_of_rows; row_number++){
        for(uint32_t column_number = 1; column_number <= self -> amount_of_columns; column_number++) {

            void* matrix_element = matrix_element_at(self, row_number, column_number);
            if(matrix_element != NULL){
                (*matrix_element_printer) (matrix_element);
            }
            else{
                printf("%10s", "Empty");
            }
        }
        printf("\n");
    }
}

void matrix_destroy(t_matrix* self){

    for(uint32_t row_index = 0; row_index < self -> amount_of_rows; row_index++){
        free((self -> data)[row_index]);
    }
    free(self -> data);
    free(self);
}

void matrix_destroy_and_destroy_elements(t_matrix* self, void (*data_destroyer) (void*)){
    matrix_iterate_doing(self, data_destroyer);
    matrix_destroy(self);
}