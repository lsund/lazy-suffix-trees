#ifndef OCCURS_H
#define OCCURS_H

#include "stree.h"
#include "types.h"
#include "basedef.h"
#include "bitvector.h"
#include "sort.h"
#include "eval.h"
#include "array.h"

typedef struct result {
    bool def;
    bool val;
} Result;


typedef struct pattern {
    wchar_t head;
    wchar_t *probe;
    wchar_t *end;
} Pattern;


bool search(wchar_t *left, wchar_t *patt_end);

#endif
