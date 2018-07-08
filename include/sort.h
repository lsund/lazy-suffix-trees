#ifndef COUNTING_SORT_H
#define COUNTING_SORT_H

#include "types.h"
#include "spaceman.h"
#include "lcp.h"
#include "sortbuffer.h"


// Sorts the suffixes bounded between left and right in `text.ss` using
// counting sort.
void counting_sort(Wchar **left, Wchar **right);

// Initialize the suffix array used for sorting by inserting the suffixes in
// slots according to their first character.
void initialize_suffixes();


#endif
