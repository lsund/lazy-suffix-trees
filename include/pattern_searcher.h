#ifndef PATTERN_SEARCHER_H
#define PATTERN_SEARCHER_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <string.h>

#include "basedef.h"
#include "types.h"
#include "spaceman.h"
#include "bitvector.h"
#include "io.h"
#include "util.h"
#include "spaceman.h"
#include "lcp.h"
#include "search.h"
#include "clock.h"
#include "reverse.h"

///////////////////////////////////////////////////////////////////////////////
// Functions


bool search_pattern(wchar_t *current_pattern, Uint patternlen);

void search_patterns(const char *path, int npatterns, wchar_t ***o_patterns);

#endif
