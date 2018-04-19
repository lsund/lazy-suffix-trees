
#ifndef SPACEMAN_H
#define SPACEMAN_H

#include <string.h>
#include "types.h"
#include "basedef.h"
#include "stree.h"


///////////////////////////////////////////////////////////////////////////////
// Macros


// A branching node can have at most MAX_CHARS outgoing edges. Thus, the suffix
// tree must be extended by at least MAX_CHARS integers.
#define EXTENSION_SIZE            (BRANCHWIDTH * (MAX_CHARS + 1) + 1)


#define ALLOC(V,S,T,N)\
        V = (T *) realloc(S, sizeof(T) * (size_t) (N));\
        if((V) == NULL)\
        {\
            fprintf(stderr, "Colud not allocate enough space");\
            exit(EXIT_FAILURE);\
        }


#define FREE(P)\
        if((P) != NULL)\
        {\
            free(P);\
            P = NULL;\
        }

///////////////////////////////////////////////////////////////////////////////
// Functions

wchar_t **alloc_sortbuffer(wchar_t **left, wchar_t **right);

void alloc_extend_stree(void);

#endif
