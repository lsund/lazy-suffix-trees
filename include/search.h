#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>

#include "config.h"
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

// Query the `pattern` with length `len` in the suffix tree. This procedure also
// drives the evaluation of the vertices.
bool search(Wchar *pattern, Uint len);

// Query all patterns given by `patternsp` in the suffix tree using `search`.
void search_many(int npatterns, Wchar ***patternsp);

// Randomly sample `n` patterns from the text and query all of them. The random
// patterns are uniformly distributed over the entire file and has maximum
// length `maxlen` and minimum length `minlen`. `path` is the name of the file,
// for which the random patterns should be written to, seperated by newlines.
void search_random(const char *path, Uint trials, Uint minlen, Uint maxlen);

#endif
