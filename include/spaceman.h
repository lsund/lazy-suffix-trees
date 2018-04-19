
#ifndef SPACEMAN_H
#define SPACEMAN_H

#include <string.h>
#include "types.h"
#include "basedef.h"
#include "stree.h"


///////////////////////////////////////////////////////////////////////////////
// Macros


#define MAXSUCCSPACE            (BRANCHWIDTH * (MAX_CHARS + 1) + 1)


// Simplify calls to space managing functions
// S is the pointer
#define ALLOCSPACE(S, T, N)\
        (T *) realloc(S, sizeof(T) * (size_t) (N))


#define ALLOC(V,S,T,N)\
        V = ALLOCSPACE(S,T,N);\
        if((V) == NULL)\
        {\
          fprintf(stderr,"file %s, line %lu: malloc(%lu) failed\n",\
                  __FILE__,(Ulong) __LINE__,\
                  (Ulong) (sizeof(T) * (size_t) (N)));\
          exit(EXIT_FAILURE);\
        }


// Free the given pointer
#define FREE(P)\
        if((P) != NULL)\
        {\
          free(P);\
          P = NULL;\
        }

#define DYNAMICSTRDUP(S)\
        strdup(S)

///////////////////////////////////////////////////////////////////////////////
// Functions

wchar_t **alloc_sortbuffer(wchar_t **left, wchar_t **right);

void alloc_stree(void);

#endif
