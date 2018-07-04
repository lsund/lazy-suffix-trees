#ifndef PATTERN_H
#define PATTERN_H

#include "types.h"

// This structure represents a pattern.
typedef struct pattern {
    // Points to the start address of the pattern.
    Wchar *start;
    // Points to the end address of the pattern.
    Wchar *end;
    // The first character of the pattern
    Wchar fst;
    // A pattern can be matched against another string. This pointer points to
    // the current character being matched.
    Wchar *current;
} Pattern;

// Initialize a pattern by setting all its fields based on the given pointers.
Pattern pattern_init(Wchar *start, Wchar *end);

// Test whether the pattern has the given length.
bool pattern_has_length(Pattern patt, Uint len);

// Test whether the pattern is empty, that is, has length 0.
bool pattern_is_empty(Pattern patt);

#endif
