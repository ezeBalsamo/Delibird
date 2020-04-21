#ifndef COMMON_STRUCTURES_H
#define COMMON_STRUCTURES_H

#include <stdint.h>

typedef struct Request{
    uint32_t operation;
    void* structure;
}request;

#endif //COMMON_STRUCTURES_H
