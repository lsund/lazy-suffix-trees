#ifndef OCCURS_H
#define OCCURS_H

#include "types.h"
#include "preprocdef.h"
#include "intbits.h"
#include "counting_sort.h"
#include "eval.h"
#include "arraydef.h"

extern Uchar *text, **suffixbase, *sentinel;

extern Uint *streetab, rootchildtab[UCHAR_MAX + 1];

extern BOOL rootevaluated;


///////////////////////////////////////////////////////////////////////////////
// Functions

BOOL occurslazy(
        void *state,
        Uchar *text,
        Uint textlen,
        Uchar *leftpattern,
        Uchar *rightpattern
        );


BOOL occurseager(/*@unused@*/ void *state,Uchar *text,
                        /*@unused@*/ Uint textlen,
                        Uchar *leftpattern,Uchar *rightpattern);
#endif
