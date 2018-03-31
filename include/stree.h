
#ifndef STREE_H
#define STREE_H


///////////////////////////////////////////////////////////////////////////////
// Getters

#define NODEINDEX(N)            ((Uint) ((N) - stree))

// leaf at this address
#define LEAFBIT                 FIRSTBIT

// right most child of succ list
#define RIGHTMOSTCHILDBIT       SECONDBIT

// unevaluated branching node
#define UNEVALUATEDBIT          FIRSTBIT

// LP is the leaf set of on edge plus the length of the string leading up to it
#define GETLP(P)                ((*(P)) & ~(LEAFBIT | RIGHTMOSTCHILDBIT))

// The number for the first child of the vertex pointer
#define GETFIRSTCHILD(P)        (*((P) + 1))

// startposition of suffix
#define SUFFIXNUMBER(L)         ((Uint) (*(L) - text))

// The left boundry of the remaining suffixes
#define GETLEFTBOUNDARY(P)      (suffixbase + *(P))

// The right boundry of the remaining suffixes
#define GETRIGHTBOUNDARY(P)     (suffixbase + ((*((P) + 1)) & ~UNEVALUATEDBIT))

// The lp number of an unevaluated vertex
#define GETLPUNEVAL(N)          SUFFIXNUMBER(GETLEFTBOUNDARY(N))

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

#define ISLEAF(P)               ((*(P)) & LEAFBIT)

#define ISRIGHTMOSTCHILD(P)     ((*(P)) & RIGHTMOSTCHILDBIT)

#define ISUNEVALUATED(P)        ((*((P)+1)) & UNEVALUATEDBIT)


#endif
