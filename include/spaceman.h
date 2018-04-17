
#ifndef SPACEMAN_H
#define SPACEMAN_H

#include <string.h>
#include "types.h"
#include "basedef.h"
#include "stree.h"


///////////////////////////////////////////////////////////////////////////////
// Macros


#define MAXSUCCSPACE            (BRANCHWIDTH * (UCHAR_MAX+1) + 1)


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

//\IgnoreLatex{

#define SPACEBLOCKINFO(SP)\
        spaceblockinfo(__FILE__,(Uint) __LINE__,(void *) SP)


///////////////////////////////////////////////////////////////////////////////
// Functions

wchar_t **get_sortbuffer(wchar_t **left, wchar_t **right);

void allocstree(void);

Sint checkenvvaronoff(char *varname);
void spaceblockinfo(char *file,Uint linenum,void *ptr);
void activeblocks(void);
void checkspaceleak(void);
void showspace(void);
void showmemsize(void);
void addspace(Uint);
void subtractspace(Uint);

Filedesctype simplefileOpen(char *file,Uint line,
                            char *filename,Uint *numofbytes);

Sint deletememorymap(char *file,Uint line,void *mappedfile);
void mmcheckspaceleak(void);
void mmshowspace(void);
Sint setlocalpagesize(void);


#endif
