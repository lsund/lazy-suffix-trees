
#ifndef SPACEMAN_H
#define SPACEMAN_H

#include <string.h>
#include "types.h"
#include "debug.h"
#include "basedef.h"


///////////////////////////////////////////////////////////////////////////////
// Macros


#define MAXSUCCSPACE            (BRANCHWIDTH * (UCHAR_MAX+1) + 1)


// Simplify calls to space managing functions
#define ALLOCSPACE(S, T, N)\
        (T *) realloc(S, sizeof(T) * (size_t) (N))


#define ALLOC(V,S,T,N)\
        V = ALLOCSPACE(S,T,N);\
        if((V) == NULL)\
        {\
          fprintf(stderr,"file %s, line %lu: malloc(%lu) failed\n",\
                  __FILE__,(Showuint) __LINE__,\
                  (Showuint) (sizeof(T) * (size_t) (N)));\
          exit(EXIT_FAILURE);\
        }

#define FREESPACE(P)\
        if((P) != NULL)\
        {\
          free(P);\
          P = NULL;\
        }

#define DYNAMICSTRDUP(S)\
        strdup(S)

#define CREATEMEMORYMAP(F,WM,NB)\
        creatememorymap(__FILE__,(Uint) __LINE__,F,WM,NB)

#define CREATEMEMORYMAPFORFILEDESC(FDFILE,FD,WM,NB)\
        creatememorymapforfiledesc(__FILE__,(Uint) __LINE__,FDFILE,FD,WM,NB)

#define DELETEMEMORYMAP(MF)\
        deletememorymap(__FILE__,(Uint) __LINE__,(void *) MF)

//\IgnoreLatex{

#define SPACEBLOCKINFO(SP)\
        spaceblockinfo(__FILE__,(Uint) __LINE__,(void *) SP)


///////////////////////////////////////////////////////////////////////////////
// Globals


extern Uchar **suffixes, **sbufferspace;

extern Uint
    sbufferwidth,
    maxsbufferwidth,
    *nextfreeentry,
    *stree,
    streesize,
    textlen;


///////////////////////////////////////////////////////////////////////////////
// Functions

Uchar **getsbufferspacelazy(Uchar **left, Uchar **right);

Uchar **getsbufferspaceeager(Uchar **left, Uchar **right);

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

/*@null@*/ void *creatememorymapforfiledesc(char *file,
                                            Uint line,
                                            char *fdfile,
                                            Filedesctype fd,
                                            BOOL writemap,
                                            Uint numofbytes);
/*@null@*/ void *creatememorymap(char *file,Uint line,char *filename,
                                 BOOL writemap,Uint *numofbytes);
Sint deletememorymap(char *file,Uint line,void *mappedfile);
void mmcheckspaceleak(void);
void mmshowspace(void);
Sint setlocalpagesize(void);


#endif
