
#ifndef STREE_H
#define STREE_H

#include "types.h"
#include "bitvector.h"

///////////////////////////////////////////////////////////////////////////////
// Getters

#define INDEX(N)            ((Uint) ((N) - stree))

// Bit determining if leaf
#define LEAFBIT                 FIRSTBIT

// right most child of succ list
#define RIGHTMOSTCHILDBIT       SECONDBIT

// unevaluated branching node
#define UNEVALUATEDBIT          FIRSTBIT

// LP is the leaf set of on edge plus the length of the string leading up to it
#define GET_LP(P)                ((*(P)) & ~(LEAFBIT | RIGHTMOSTCHILDBIT))

// The number for the first child of the vertex pointer
#define FIRST_CHILD(P)        (*((P) + 1))

// startposition of suffix
#define SUFFIXNUMBER(L)         ((Uint) (*(L) - text))

// The left boundry of the remaining suffixes
#define GETLEFTBOUNDARY(P)      (suffixbase + *(P))

// The right boundry of the remaining suffixes
#define GETRIGHTBOUNDARY(P)     (suffixbase + ((*((P) + 1)) & ~UNEVALUATEDBIT))

// The lp number of an unevaluated vertex
#define GET_LP_UNEVAL(N)          SUFFIXNUMBER(GETLEFTBOUNDARY(N))

///////////////////////////////////////////////////////////////////////////////
// Setters

#define SETLP(P, LP)             *(P) = (*(P) & RIGHTMOSTCHILDBIT) | (LP)

#define SETFIRSTCHILD(P,C)      *((P)+1) = C

#define SETLEAF(P,L)            *(P) = (L) | LEAFBIT

// Store the left and right boundaries for the remaining suffixes
#define STOREBOUNDARIES(P,L,R)  *(P) = (Uint) ((L) - suffixbase);\
                                *((P)+1) = ((R) - suffixbase) | UNEVALUATEDBIT


///////////////////////////////////////////////////////////////////////////////
// Queries

#define IS_LEAF(P)              ((*(P)) & LEAFBIT)

#define IS_RIGHTMOST(P)         ((*(P)) & RIGHTMOSTCHILDBIT)

#define IS_UNEVALUATED(P)       ((*((P)+1)) & UNEVALUATEDBIT)

#endif
