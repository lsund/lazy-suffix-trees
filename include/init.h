#ifndef INIT_H
#define INIT_H

#include "types.h"
#include "debugdef.h"
#include "protodef.h"
#include "spaceman.h"
#include "fhandledef.h"


///////////////////////////////////////////////////////////////////////////////
// Fields


extern Uchar    *text,
                *sentinel,
                characters[UCHAR_MAX + 1],
                **suffixbase;

extern Uint     textlen,
                alphasize,
                streetabsize,
                suffixessize,
                maxunusedsuffixes,
                alphaindex[UCHAR_MAX + 1],
                occurrence[UCHAR_MAX + 1];

extern BOOL     rootevaluated;


///////////////////////////////////////////////////////////////////////////////
// Functions

void inittree(void);

#endif
