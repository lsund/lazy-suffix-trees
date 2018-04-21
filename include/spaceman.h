/*
 * Copyright by Stefan Kurtz (C) 1999-2003
 * =====================================
 * You may use, copy and distribute this file freely as long as you
 * - do not change the file,
 * - leave this copyright notice in the file,
 * - do not make any profit with the distribution of this file
 * - give credit where credit is due
 * You are not allowed to copy or distribute this file otherwise
 * The commercial usage and distribution of this file is prohibited
 * Please report bugs and suggestions to <kurtz@zbh.uni-hamburg.de>
 *
 * ======================================
 *
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz
 * For full source control tree, see https://github.com/lsund/wotd
 *
 */

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

Wchar **alloc_sortbuffer(Wchar **left, Wchar **right);

void alloc_extend_stree(void);

#endif
