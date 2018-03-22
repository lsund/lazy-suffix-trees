#ifndef PREPROCDEF_H
#define PREPROCDEF_H

#define BRANCHWIDTH             UintConst(2)

#define NODEINDEX(N)        ((Uint) ((N) - streetab))

#define LEAFBIT             FIRSTBIT // leaf at this address
#define RIGHTMOSTCHILDBIT   SECONDBIT // right most child of succ list
#define UNEVALUATEDBIT      FIRSTBIT // unevaluated branching node

#define MAXTEXTLEN          UintConst(715827882)


#define ISLEAF(P)           ((*(P)) & LEAFBIT)
#define ISRIGHTMOSTCHILD(P) ((*(P)) & RIGHTMOSTCHILDBIT)
#define ISUNEVALUATED(P)    ((*((P)+1)) & UNEVALUATEDBIT)


#define GETLP(P)            ((*(P)) & ~(LEAFBIT | RIGHTMOSTCHILDBIT))
#define GETFIRSTCHILD(P)    (*((P)+1))
#define SETLP(P,LP)         *(P) = (*(P) & RIGHTMOSTCHILDBIT) | (LP)
#define SETFIRSTCHILD(P,C)  *((P)+1) = C
#define SETLEAF(P,L)        *(P) = (L) | LEAFBIT


#define UNDEFREFERENCE        (UINT_MAX)      // undefined reference

#define SUFFIXNUMBER(L)        ((Uint) (*(L) - text))  // startposition of suffix

#define STOREBOUNDARIES(P,L,R) *(P) = (Uint) ((L) - suffixbase);\
                               *((P)+1) = ((R) - suffixbase) | UNEVALUATEDBIT
#define GETLEFTBOUNDARY(P)     (suffixbase + *(P))
#define GETRIGHTBOUNDARY(P)    (suffixbase + ((*((P)+1)) & ~UNEVALUATEDBIT))

#define GETLPUNEVAL(N) SUFFIXNUMBER(GETLEFTBOUNDARY(N))

#define UNDEFINEDSUCC  (UINT_MAX)    // undefined successor

#endif
