#ifndef DELIBIRD_QUEUE_CODE_NAME_ASSOCIATIONS_H
#define DELIBIRD_QUEUE_CODE_NAME_ASSOCIATIONS_H

#include <stdint.h>
#include <stdbool.h>

void initialize_queue_code_name_associations();

char* queue_name_of(uint32_t queue_code);
uint32_t queue_code_of(char* queue_name);
bool exists_queue_named(char* queue_name);
void free_queue_code_name_associations();

#endif //DELIBIRD_QUEUE_CODE_NAME_ASSOCIATIONS_H
