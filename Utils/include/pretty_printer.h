#ifndef DELIBIRD_PRETTY_PRINTER_H
#define DELIBIRD_PRETTY_PRINTER_H

#include <stdint.h>
#include "common_structures.h"

void initialize_pretty_printer();

char* pretty_print_of(uint32_t code, void* structure);

char* request_pretty_print(t_request* request);

void free_pretty_printer();

#endif //DELIBIRD_PRETTY_PRINTER_H
