/*
  Copyright by Stefan Kurtz (C) 1997-2003
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

#ifndef DEBUG_H
#define DEBUG_H

#include <stdlib.h>
#include <stdio.h>
#include "basedef.h"
#include "bitvector.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include "types.h"
#include "io.h"
#include "debug.h"
#include "error.h"

extern Uchar *text, *sentinel, characters[UCHAR_MAX + 1];
extern Uint     rootchildtab[UCHAR_MAX + 1],
                textlen,
                *stree,
                *nextfreeentry,
                alphasize;

///////////////////////////////////////////////////////////////////////////////
// Functions

void showrootchildtab(void);

void showsubtree(Uint *father,int indent);
/*
  This file defines macros for producing debugging messages. Except for
  the first two macros, all macros are only defined in case
  \texttt{DEBUG} is defined.
*/

/*
  \texttt{STAMP} shows the file and the line number, where the macro is
  placed. Sometimes this should only be done if a condition is satisfied.
  So we define a macro \texttt{CONDSTAMP}.
*/

#define STAMP\
        printf("STAMP(%lu,%s)\n",(Showuint) __LINE__,__FILE__);\
        (void) fflush(stdout)

#define CONDSTAMPC(C) if(C){STAMP;}

//\IgnoreLatex{

#ifdef DEBUG

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

//}

/*
  The output of the debug macros is performed according to the
  value of environment variable \texttt{DEBUGLEVEL} and
  \texttt{DEBUGWHERE}. We allow a debug level from 0 to 6.
*/

#define MAXDEBUGLEVEL 6

/*
  The general rule of using debug levels is as follows: the larger the
  debug level, the more messages will be printed. The debug level
  of the message to be printed should be set according
  to the following (very informal) rules:
  \begin{itemize}
  \item
  \texttt{DEBUGLEVEL} undefined means no output
  \item
  \texttt{DEBUGLEVEL=0} means no output
  \item
  \texttt{DEBUGLEVEL=1} checking, and if something wrong report it
  \item
  \texttt{DEBUGLEVEL=2} some important statistics
  \item
  \texttt{DEBUGLEVEL=3} output of the result
  \item
  \texttt{DEBUGLEVEL=4} output intermediate steps
  \item
  \texttt{DEBUGLEVEL=5} some less important steps
  \item
  \texttt{DEBUGLEVEL=6} some even less important steps
  \end{itemize}
  In addition \texttt{DEBUGLEVEL},
  there are two environment variables that trigger the format
  of the debug message:
  \begin{itemize}
  \item
  \texttt{DEBUGWHERE} undefined means no file name and line number
  \item
  \texttt{DEBUGWHERE=off} means no file name and line number
  \item
  \texttt{DEBUGWHERE=on} means to show file and line number in C-Program
  where macro is used.
  \end{itemize}
*/

/*
  \texttt{GENDEBUG} is a generic macro to define the debug macros
  of different arity.
  \texttt{L} is the \texttt{debuglevel} according to the
  environment variable \texttt{DEBUGLEVEL}. We check if
  the variable \texttt{L} is in the range
  \texttt{[0,MAXDEBUGLEVEL]}. If \texttt{L}
  is smaller than the \texttt{debuglevel} and \texttt{debugwhere}
  is true, then the file and line number is printed. Otherwise, we
  only check the \texttt{debuglevel}. Only if \texttt{L} is smaller than the
  \texttt{debuglevel}, the function
  \texttt{fprintf} is called with the appropriate format string and the given
  arguments. We use \texttt{fprintf} to do the debug output, since it
  allows the compiler to check if the arguments are consistent with the
  format string.
*/

#define GENDEBUG(L)\
        if((L) <= 0 || (L) > MAXDEBUGLEVEL)\
        {\
          fprintf(getdbgfp(),\
                  "file \"%s\", line %lu: level n [1..%lu] required\n",\
                  __FILE__,(Showuint) __LINE__,(Showuint) MAXDEBUGLEVEL);\
          exit(EXIT_FAILURE);\
        }\
        if(((Sint) (L)) <= getdebuglevel() && getdebugwhere())\
        {\
          fprintf(getdbgfp(),"file \"%s\", line %lu: ",__FILE__,\
                  (Showuint) __LINE__);\
        }\
        if(((Sint) (L)) <= getdebuglevel())

/*
   \texttt{GENDEBUG} is used as a prefix of the
   \texttt{fprintf}-commands followed by an \texttt{fflush}.
   The following macros should always be used for printing debugging info.
*/

#define DEBUG0(L,F)\
        GENDEBUG(L){fprintf(getdbgfp(),F); (void) fflush(stdout);}
#define DEBUG1(L,F,A1)\
        GENDEBUG(L){fprintf(getdbgfp(),F,A1); (void) fflush(stdout);}
#define DEBUG2(L,F,A1,A2)\
        GENDEBUG(L){fprintf(getdbgfp(),F,A1,A2); (void) fflush(stdout);}
#define DEBUG3(L,F,A1,A2,A3)\
        GENDEBUG(L){fprintf(getdbgfp(),F,A1,A2,A3); (void) fflush(stdout);}
#define DEBUG4(L,F,A1,A2,A3,A4)\
        GENDEBUG(L){fprintf(getdbgfp(),F,A1,A2,A3,A4); (void) fflush(stdout);}
#define DEBUG5(L,F,A1,A2,A3,A4,A5) \
        GENDEBUG(L){fprintf(getdbgfp(),F,A1,A2,A3,A4,A5); (void) fflush(stdout);}

/*
  The following macros are abbreviations for calling functions that set
  the debug level. The macros must appear in the program text
  before the first debug command. The other macros create and close an
  extra file to write the debug messages to.
*/

#define DEBUGLEVELSET            setdebuglevel()
#define DEBUGCLOSEFILE           debugclosefile()

/*
  The following macros are used to declare local variables for debugging
  purposes and to call a function depending on the \texttt{DEBUGLEVEL}.
*/

#define DEBUGDECL(S)   S
#define DEBUGCODE(L,S) if(((Sint) (L)) <= getdebuglevel())\
                       {\
                         S;\
                       }

/*
  To follow the program flow the following macros can be used.
*/

#define CASE(I)       fprintf(getdbgfp(),"Case(%ld)\n",(Showsint) (I)); (void) fflush(stdout)
#define CASELINE(I)   fprintf(getdbgfp(),"file %s, line %lu: Case(%ld)\n",\
                              __FILE__,(Showuint) __LINE__,(Showuint) (I))

#define COMPAREUINTVALUES(VAL1,VAL2)\
        if((VAL1) != (VAL2))\
        {\
          fprintf(stderr,"%s, line %lu: %s = %lu != %lu = %s\n",\
                          __FILE__,\
                          (Showuint) __LINE__,\
                          #VAL1,\
                          (Showuint) (VAL1),\
                          (Showuint) (VAL2),\
                          #VAL2);\
          exit(EXIT_FAILURE);\
        }

/*
  Some forward declarations of the functions used for producing
  debugging messages.
*/

//\IgnoreLatex{

#ifdef __cplusplus
  extern "C" {
#endif

//}

///////////////////////////////////////////////////////////////////////////////
// Functions
Sint getdebuglevel(void);
Bool getdebugwhere(void);
void showmemsize(void);
void setdebuglevel(void);
FILE *getdbgfp(void);
void debugclosefile(void);

void printAlpha (char *s);

void showtree(void);
void showstree(void);
void showpatternstat(Uint *patternstat);

    //\IgnoreLatex{

#ifdef __cplusplus
}
#endif

#else

/*
  This the the case where \texttt{DEBUG} is not defined.
  Hence all debug-macros are ignored by the preprocessor.
*/

#define DEBUG0(L,F)                /* nothing */
#define DEBUG1(L,F,A1)             /* nothing */
#define DEBUG2(L,F,A1,A2)          /* nothing */
#define DEBUG3(L,F,A1,A2,A3)       /* nothing */
#define DEBUG4(L,F,A1,A2,A3,A4)    /* nothing */
#define DEBUG5(L,F,A1,A2,A3,A4,A5) /* nothing */
#define DEBUGDECL(S)               /* nothing */
#define DEBUGCODE(L,S)             /* nothing */
#define DEBUGLEVELSET              /* nothing */
#define DEBUGCLOSEFILE(FP)         /* nothing */
#define CASE(I)                    /* nothing */
#define CASELINE(I)                /* nothing */
#define COMPAREUINTVALUES(VAL1,VAL2) /* nothing */

#endif  /* DEBUG */

#endif

//}
