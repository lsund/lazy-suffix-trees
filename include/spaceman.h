
#ifndef SPACEMAN_H
#define SPACEMAN_H

#include "types.h"
#include "debugdef.h"
#include "spacedef.h"
#include "preprocdef.h"

///////////////////////////////////////////////////////////////////////////////
// Globals

extern Uchar **suffixes, **sbufferspace;

extern Uint
    sbufferwidth,
    maxsbufferwidth,
    *nextfreeentry,
    *stree,
    streesize,
    textlen;


///////////////////////////////////////////////////////////////////////////////
// Functions

Uchar **getsbufferspacelazy(Uchar **left, Uchar **right);

Uchar **getsbufferspaceeager(Uchar **left, Uchar **right);

void allocstree(void);

#endif
