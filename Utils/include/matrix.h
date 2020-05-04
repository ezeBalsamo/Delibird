#ifndef DELIBIRD_MATRIX_H
#define DELIBIRD_MATRIX_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Matrix{
    uint32_t amount_of_rows;
    uint32_t amount_of_columns;
    void*** data;
    bool must_be_square;
    bool is_expandable;
    bool must_reorder_after_insert;
}t_matrix;

t_matrix* matrix_create(uint32_t amount_of_rows, uint32_t amount_of_columns, bool should_be_expandable, bool should_be_squared, bool should_reorder_after_insert);
void insert_matrix_element_at(t_matrix* self, void* element, uint32_t row_index, uint32_t column_index);
void* matrix_element_at(t_matrix* self, uint32_t row_index, uint32_t column_index);
void print_matrix(t_matrix* self, void (matrix_element_printer) (void*));
void matrix_destroy(t_matrix* self);
void matrix_destroy_and_destroy_elements(t_matrix* matrix, void (*data_destroyer) (void*));

#endif //DELIBIRD_MATRIX_H
