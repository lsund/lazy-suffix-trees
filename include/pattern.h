#ifndef PATTERN_H
#define PATTERN_H

#include "types.h"

typedef struct pattern {
    Wchar head;
    Wchar *cursor;
    Wchar *start;
    Wchar *end;
} Pattern;

Pattern init_pattern(Wchar *patt_start, Wchar *patt_end);

#endif
