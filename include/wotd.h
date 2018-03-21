#ifndef WOTD_H
#define WOTD_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <string.h>
#include "types.h"
#include "debugdef.h"
#include "spacedef.h"
#include "experim.h"
#include "intbits.h"
#include "args.h"
#include "arraydef.h"
#include "fhandledef.h"
#include "protodef.h"
#include "preprocdef.h"
#include "counting_sort.h"
#include "util.h"
#include "spaceman.h"

void inittree(void);

void evaluateeager(void);

void wotd(BOOL evaleager, char ***patterns_p, int npatterns);

#endif
