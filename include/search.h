#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>

#include "basedef.h"
#include "types.h"
#include "spaceman.h"
#include "io.h"
#include "util.h"
#include "spaceman.h"
#include "lcp.h"
#include "search.h"
#include "clock.h"
#include "pattern.h"
#include "match.h"
#include "stree.h"

bool search(Wchar *current_pattern, Uint patternlen);

void search_many(const char *path, int npatterns, Wchar ***o_patterns);

// Randomly sampling patterns from the text, reversing every second to simulate
// the case where a pattern does not exist.
void search_random(const char *path, Uint trials, Uint minlen, Uint maxlen);

#endif
