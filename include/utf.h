
#ifndef UTF8_H
#define UTF8_H

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

int from_utf8(Uchar **pstart, Uchar *end, Uint *pcp);

#endif
