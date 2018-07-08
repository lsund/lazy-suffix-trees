#ifndef SPACEMAN_H
#define SPACEMAN_H

#include <string.h>
#include "types.h"
#include "config.h"
#include "stree.h"
#include "streedef.h"
#include "sortbuffer.h"


///////////////////////////////////////////////////////////////////////////////
// Macros


// A branching node can have at most MAX_CHARS outgoing edges. Thus, the suffix
// tree must be extended by at least MAX_CHARS integers.
#define EXTENSION_SIZE            (INNER_VERTEXSIZE * (MAX_CHARS + 1) + 1)


#define ALLOC(V,T,N)\
        V = (T *) realloc(V, sizeof(T) * (size_t) (N));\
        if((V) == NULL)\
        {\
            fprintf(stderr, "Could not allocate enough space\n");\
            exit(EXIT_FAILURE);\
        }

// Perform a null check, then free P
#define FREE(P)\
        if((P) != NULL)\
        {\
            free(P);\
            P = NULL;\
        }

// Allocates enough space for sorting the suffixes delimited by `left` and
// `right`
Wchar **alloc_sortbuffer(Wchar **left, Wchar **right);

// Dynamically allocate more space for the vertex table `st.vs`
void alloc_extend_stree(void);

#endif
