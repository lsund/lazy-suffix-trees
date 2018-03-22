#ifndef EVAL_H
#define EVAL_H

#include "failures.h"
#include "intbits.h"
#include "types.h"
#include "preprocdef.h"
#include "spaceman.h"
#include "lcp.h"
#include "counting_sort.h"

///////////////////////////////////////////////////////////////////////////////
// Macros


#define NOTSTACKEMPTY         (stacktop > 0)

#define PUSHNODE(N)\
        if(stacktop >= stackalloc)\
        {\
          stackalloc += 100;\
          ALLOC(stack,stack,Uint,stackalloc);\
        }\
        DEBUGCODE(1,if(stacktop > maxstacksize) { maxstacksize = stacktop;});\
        NOTSUPPOSEDTOBENULL(stack);\
        stack[stacktop++] = N

#define POPNODE(N)\
        N = stack[--stacktop]



///////////////////////////////////////////////////////////////////////////////
// Functions


Uint evalrootsuccedges(Uchar **left,Uchar **right);

void evaluatenodelazy(Uint node);

void evaluateeager(void);


///////////////////////////////////////////////////////////////////////////////
// Fields


extern Uchar *text, *sentinel, **suffixbase, **sbuffer;

extern Uint
    *nextfreeentry,
    rootchildtab[UCHAR_MAX + 1], suffixessize, maxunusedsuffixes;

#endif
