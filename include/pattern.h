#ifndef PATTERN_H
#define PATTERN_H

#include "types.h"

typedef struct pattern {
    Wchar head;
    Wchar *current;
    Wchar *start;
    Wchar *end;
} Pattern;

Pattern pattern_init(Wchar *patt_start, Wchar *patt_end);

bool pattern_has_length(Pattern patt, Uint len);

bool pattern_is_empty(Pattern patt);

#endif
