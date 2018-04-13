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

#ifndef ERRORDEF_H
#define ERRORDEF_H
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

// The macros in this file write error messages into a buffer returned by
// this funtion
char *messagespace(void);

Sint maxerrormsg(void);

void seterror(Sint code);

Sint geterror(void);

void reseterror(void);

#define ERROR(S);\
    fprintf(stderr, "Error: %s\n", S);\
    exit(EXIT_FAILURE);

#define GENERROR(C);\
        if(((Sint) (C)) >= maxerrormsg())\
        {\
          fprintf(stderr,"file %s, line %lu: "\
                         "space for errormessage too small\n",\
                  __FILE__,(Ulong) __LINE__);\
          exit(EXIT_FAILURE);\
        }

#define ERROR0(F)\
        GENERROR(sprintf(messagespace(),F))

#define ERROR1(F,A1)\
        GENERROR(sprintf(messagespace(),F,A1))

#define ERROR2(F,A1,A2)\
        GENERROR(sprintf(messagespace(),F,A1,A2))

#define ERROR3(F,A1,A2,A3)\
        GENERROR(sprintf(messagespace(),F,A1,A2,A3))

#define ERROR4(F,A1,A2,A3,A4)\
        GENERROR(sprintf(messagespace(),F,A1,A2,A3,A4))

#define ERROR5(F,A1,A2,A3,A4,A5)\
        GENERROR(sprintf(messagespace(),F,A1,A2,A3,A4,A5))

#define ERROR6(F,A1,A2,A3,A4,A5,A6)\
        GENERROR(sprintf(messagespace(),F,A1,A2,A3,A4,A5,A6))

#define ERROR7(F,A1,A2,A3,A4,A5,A6,A7)\
        GENERROR(sprintf(messagespace(),F,A1,A2,A3,A4,A5,A6,A7))


// The following is a macro to show the usage line for all programs
// which have options and an indexname as the last argument.

#define USAGEOUT\
        ERROR2("Usage: %s options indexname\n"\
               "%s -help shows possible options",\
                argv[0], argv[0]);

// The following is the standard message in the main function. It shows the
// program name and the error message as returned by the function
// \texttt{messagespace}.

#define STANDARDMESSAGE\
        fprintf(stderr,"%s: %s\n",argv[0],messagespace());\
        return EXIT_FAILURE

#define SIMPLESTANDARDMESSAGE\
        fprintf(stderr,"%s\n",messagespace());\
        return EXIT_FAILURE

#if !defined __func__ && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 199901L)
# if (defined __cplusplus && defined __GNUC__ && __GNUC__ >= 2) || defined __PRETTY_FUNCTION__
#  define __func__  __PRETTY_FUNCTION__
# elif (defined __GNUC__ && __GNUC__ >= 2) || defined __FUNCTION__
#  define __func__  __FUNCTION__
# else
#  define __func__  __FILE__
# endif
#endif

#define FUNCTIONCALL /*@ignore@*/DEBUG1(3,"call %s\n",__func__)/*@end@*/

#define FUNCTIONFINISH /*@ignore@*/DEBUG1(3,"done with %s\n",__func__)/*@end@*/


// Failures

#define NOTSUPPOSED\
        fprintf(stderr,"%s: line %lu: This case is not supposed to occur\n",\
                       __FILE__,(Ulong) __LINE__);\
        exit(EXIT_FAILURE)

#define DEFAULTFAILURE default:\
                       fprintf(stderr, "line %lu in file %s: this point "\
                                       "should never be reached; aborting "\
                                       "program\n" , (Ulong) __LINE__,\
                                       __FILE__);\
                       exit(EXIT_FAILURE)


#define NOTIMPLEMENTED\
        fprintf(stderr, "file %s, line %lu: this case is not implemented\n",\
                __FILE__,(Ulong) __LINE__);\
        exit(EXIT_FAILURE)

/*
  The following macro checks a ptr. If it is \texttt{NULL}, then the
  program terminates with an error.
*/

#ifdef DEBUG
#define NOTSUPPOSEDTOBENULL(PTR)\
        if((PTR) == NULL)\
        {\
          NOTSUPPOSED;\
        }
#else
#define NOTSUPPOSEDTOBENULL(PTR) /* Nothing */
#endif
#endif
