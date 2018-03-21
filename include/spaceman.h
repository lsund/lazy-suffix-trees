
#ifndef SPACEMAN_H
#define SPACEMAN_H

#include "types.h"
#include "debugdef.h"
#include "spacedef.h"

extern Uchar **suffixes, **sbufferspace;

extern Uint sbufferwidth, maxsbufferwidth;

Uchar **getsbufferspacelazy(Uchar **left, Uchar **right);

Uchar **getsbufferspaceeager(Uchar **left, Uchar **right);

#endif
