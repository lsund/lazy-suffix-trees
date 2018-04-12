#ifndef SEARCH_H
#define SEARCH_H

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
#include "occurs.h"
#include "search.h"
#include "clock.h"
#include "utf.h"
#include "char.h"
#include "reverse.h"

///////////////////////////////////////////////////////////////////////////////
// Functions


void search_patterns(const char *path, int npatterns, char ***o_patterns);

void search_benchmark(const char *path, Uint trials, Uint minpat, Uint maxpat);

#endif
