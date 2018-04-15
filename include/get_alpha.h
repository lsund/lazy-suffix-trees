#ifndef GET_ALPHA_H
#define GET_ALPHA_H

#include <stdio.h>
#include <limits.h>
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>


#include "types.h"
#include "basedef.h"

void get_characters(Uchar *text, Uint textlen, Uchar *alpha, Uint *alphasize);

void get_wcharacters(FILE *in, wchar_t *text, Uint textlen, wchar_t *alpha, Uint alphasize);

#endif
