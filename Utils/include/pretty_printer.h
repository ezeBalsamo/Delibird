#ifndef DELIBIRD_PRETTY_PRINTER_H
#define DELIBIRD_PRETTY_PRINTER_H

#include <stdint.h>
#include "common_structures.h"

typedef struct Printable_Object{
    uint32_t code;
    char* (*print_function) (void* object);
}t_printable_object;

void initialize_pretty_printer();

char* pretty_print_of(uint32_t code, void* structure);

char* request_pretty_print(t_request* request);

void free_pretty_printer();

#endif //DELIBIRD_PRETTY_PRINTER_H
