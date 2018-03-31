#ifndef PREPROCDEF_H
#define PREPROCDEF_H


///////////////////////////////////////////////////////////////////////////////
// Fields

#define BRANCHWIDTH             UintConst(2)

#define MAXTEXTLEN              UintConst(715827882)

#define MAXPATTERNLEN           1024

// undefined reference
#define UNDEFREFERENCE          (UINT_MAX)

// undefined successor
#define UNDEFINEDSUCC           (UINT_MAX)

///////////////////////////////////////////////////////////////////////////////
// Getters


#define NODEINDEX(N)            ((Uint) ((N) - stree))

// leaf at this address
#define LEAFBIT                 FIRSTBIT
// right most child of succ list
#define RIGHTMOSTCHILDBIT       SECONDBIT
// unevaluated branching node
#define UNEVALUATEDBIT          FIRSTBIT

#define GETLP(P)                ((*(P)) & ~(LEAFBIT | RIGHTMOSTCHILDBIT))
#define GETFIRSTCHILD(P)        (*((P)+1))

#define SUFFIXNUMBER(L)         ((Uint) (*(L) - text))  // startposition of suffix

#define GETLEFTBOUNDARY(P)      (suffixbase + *(P))

#define GETRIGHTBOUNDARY(P)     (suffixbase + ((*((P)+1)) & ~UNEVALUATEDBIT))

#define GETLPUNEVAL(N)          SUFFIXNUMBER(GETLEFTBOUNDARY(N))

// Transform bytes into megabytes
#define MEGABYTES(V)  ((double) (V)/((UintConst(1) << 20) - 1))

///////////////////////////////////////////////////////////////////////////////
// Setters


#define SETLP(P,LP)             *(P) = (*(P) & RIGHTMOSTCHILDBIT) | (LP)

#define SETFIRSTCHILD(P,C)      *((P)+1) = C

#define SETLEAF(P,L)            *(P) = (L) | LEAFBIT

#define STOREBOUNDARIES(P,L,R)  *(P) = (Uint) ((L) - suffixbase);\
                                *((P)+1) = ((R) - suffixbase) | UNEVALUATEDBIT

///////////////////////////////////////////////////////////////////////////////
// Queries


#define ISLEAF(P)               ((*(P)) & LEAFBIT)

#define ISRIGHTMOSTCHILD(P)     ((*(P)) & RIGHTMOSTCHILDBIT)

#define ISUNEVALUATED(P)        ((*((P)+1)) & UNEVALUATEDBIT)

#endif
