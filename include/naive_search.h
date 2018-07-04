#ifndef NAIVE_SEARCH_H
#define NAIVE_SEARCH_H

#include "text.h"
#include "types.h"
#include <stdbool.h>

// Note: This module has nothing to do with suffix trees and is for testing
// purposes only.

// Naively search `text.fst` for the pattern beginning at address `start`, ending
// at address `end`.
bool naive_search(Wchar *pattern, Wchar *patternright);

// Naively search `text.fst` for all occurences of the pattern beginning at
// address `start`, ending at address `end`. The result is stored in `numbers`
Uint naive_find_all(Wchar *start, Wchar *end, Uint *numbers);

#endif
