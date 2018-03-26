/*
  Copyright by Stefan Kurtz (C) 2000-2003
  =====================================
  You may use, copy and distribute this file freely as long as you
   - do not change the file,
   - leave this copyright notice in the file,
   - do not make any profit with the distribution of this file
   - give credit where credit is due
  You are not allowed to copy or distribute this file otherwise
  The commercial usage and distribution of this file is prohibited
  Please report bugs and suggestions to <kurtz@zbh.uni-hamburg.de>
*/

//\IgnoreLatex{

#ifndef SPACEDEF_H
#define SPACEDEF_H
#include <string.h>
#include "types.h"

Sint checkenvvaronoff(char *varname);
void spaceblockinfo(char *file,Uint linenum,void *ptr);
void activeblocks(void);
void checkspaceleak(void);
void showspace(void);
Uint getspacepeak(void);
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
Uint mmgetspacepeak(void);
Sint setlocalpagesize(void);

#ifdef __cplusplus
}
#endif

#define MAXSUCCSPACE            (BRANCHWIDTH * (UCHAR_MAX+1) + 1)

//}

/*
  This file defines macros to simplify the calls to the
  functions
  \begin{itemize}
  \item
  \texttt{creatememorymapforfiledesc},
  \item
  \texttt{creatememorymap},
  \item
  \texttt{delete\-memorymap}.
  \end{itemize}

  \begin{enumerate}
  \item
    The first parameter to \texttt{ALLOCSPACE} is \texttt{NULL}
    or a pointer to a previously
    allocated space block.
  \item
  The second argument of the macro is the type of the space block
  to be allocated.
  \item
  The third argument is the number of elements of that type to be
  allocated space for.
  \end{enumerate}
*/

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

#endif

//}
