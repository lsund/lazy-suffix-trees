#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stdlib.h>

typedef struct {
    int *array;
    size_t used;
    size_t size;
} Array;

#endif
