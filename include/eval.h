#ifndef EVAL_H
#define EVAL_H

#include "externs.h"
#include "error.h"
#include "bitvector.h"
#include "types.h"
#include "basedef.h"
#include "spaceman.h"
#include "lcp.h"
#include "sort.h"

///////////////////////////////////////////////////////////////////////////////
// Macros


#define NOTSTACKEMPTY         (stacktop > 0)

#define PUSHNODE(N)\
        if(stacktop >= stackalloc)\
        {\
          stackalloc += 100;\
          ALLOC(stack,stack,Uint,stackalloc);\
        }\
        NOTSUPPOSEDTOBENULL(stack);\
        stack[stacktop++] = N

#define POPNODE(N)\
        N = stack[--stacktop]



///////////////////////////////////////////////////////////////////////////////
// Functions


Uint evalrootsuccedges(wchar_t **left,wchar_t **right);

void eval_node(Uint node);

void evaluateeager(void);


#endif
