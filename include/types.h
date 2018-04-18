/*
  Copyright by Stefan Kurtz (C) 1998-2003
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

#ifndef TYPES_H
#define TYPES_H
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// This is the assumptions about the types
// size(Uint) >= 4
// size(Sint) >= 4
// size(Ushort) = 2
// size(Sshort) = 2


// Primitives
typedef unsigned char  Uchar;
typedef unsigned short Ushort;

typedef unsigned long  Uint;
typedef signed   long  Sint;

typedef unsigned long  Ulong;
typedef signed   long  Slong;

#define LOGWORDSIZE    6
#define UintConst(N)   (N##UL)
#define SintConst(N)   (N##L)

typedef int Ctypeargumenttype;      // \Typedef{Ctypeargumenttype}

typedef int Qsortcomparereturntype; // \Typedef{Qsortcomparefunction}

typedef int Fieldwidthtype;         // \Typedef{Fieldwidthtype}

typedef int Argctype;               // \Typedef{Argctype}

typedef int Getrlimitreturntype;    // \Typedef{Getrlimitreturntype}

/*
  We have output functions of different arity, all accepting integer values
*/

typedef Sint(*Outputfunction1)(void *,Uint);
typedef Sint(*Outputfunction2)(void *,Uint,Uint);
typedef Sint(*Outputfunction)(void *,Uint,Uint,Uint);

/*
  This is the type of arguments for function sysconf.
*/


#ifdef WITHSYSCONF
typedef int Sysconfargtype;         // \Typedef{Sysconfargtype}
#endif

//\IgnoreLatex{

#define CHECKTYPESIZE(T,OP,S)\
        if(sizeof(T) OP (S))\
        {\
          DEBUG4(1,"# sizeof(%s) %s (%ld bytes,%ld bits) as expected\n",\
                  #T,#OP,(Slong) sizeof(T),\
                         (Slong) (CHAR_BIT * sizeof(T)));\
        } else\
        {\
          fprintf(stderr,"typesize constraint\n");\
          fprintf(stderr,"  sizeof(%s) = (%ld bytes,%ld bits) %s %lu bytes\n",\
                  #T,\
                  (Slong) sizeof(T),\
                  (Slong) (CHAR_BIT * sizeof(T)),\
                  #OP,\
                  (Ulong) (S));\
          fprintf(stderr,"does not hold\n");\
          exit(EXIT_FAILURE);\
        }

/*
  The following function checks some type constraints
*/

#define CHECKALLTYPESIZES\
        CHECKTYPESIZE(char,==,(size_t) 1)\
        CHECKTYPESIZE(short,==,(size_t) 2)\
        CHECKTYPESIZE(int,==,(size_t) 4)\
        CHECKTYPESIZE(long,>=,(size_t) 4)\
        CHECKTYPESIZE(void *,>=,(size_t) 4)

//}

/*
  Here is a prototype for the main function.
*/

#define MAINFUNCTION int main(Argctype argc,char *argv[])

#define MAINFUNCTIONWITHUNUSEDARGUMENTS\
        int main(/*@unused@*/ Argctype argc,/*@unused@*/ char *argv[])

//\IgnoreLatex{

#ifndef __cplusplus
int mkstemp(char *);
#endif

//}

/*
  Show a boolean value as a string or as a character 0 or 1.
*/

#define SHOWbool(B) ((B) ? "true" : "false")
#define SHOWBIT(B)  ((B) ? '1' : '0')

/*
  Pairs, triples, and quadruples of unsigned integers.
*/

typedef struct
{
  Uint uint0, uint1;
} PairUint;                // \Typedef{PairUint}

typedef struct
{
  Uint uint0, uint1, uint2;
} ThreeUint;               // \Typedef{ThreeUint}

typedef struct
{
  Uint uint0, uint1, uint2, uint3;
} FourUint;                // \Typedef{FourUint}

//\IgnoreLatex{

/*
  A list is stored with its start position in some space block
  and its length.
*/

typedef struct
{
  Uint start, length;
} Listtype;                // \Typedef{Listtype}

/*
  A string is just a list.
*/

typedef Listtype Stringtype;    // \Typedef{Stringtype}

/*
  The following type stores all information necessary for a sequence
  of encoded intervals.
*/

typedef struct
{
  Uint uintvalue;
  bool defined;
} DefinedUint;   // \Typedef{DefinedUint}

/*
  The default type for length-values is unsigned int.
*/

#ifndef LENGTHTYPE
#define LENGTHTYPE Uint
#endif

/*
  The default number of bytes in a bitvector used for dynamic programming
  is 4.
*/

#ifndef DPBYTESINWORD
#define DPBYTESINWORD 4
#endif

/*
  The number of bytes in a dynamic programming bitvector determines the type
  of integers, the dp-bits are stored in.
*/

#if DPBYTESINWORD == 1
typedef unsigned char DPbitvector;          // \Typedef{DPbitvector}
#else
#if DPBYTESINWORD == 2
typedef unsigned short DPbitvector;
#else
#if DPBYTESINWORD == 4
typedef unsigned int DPbitvector;
#else
#if DPBYTESINWORD == 8
typedef unsigned long long DPbitvector;
#endif
#endif
#endif
#endif

//}

/*
  The following type stores filenames and the length of the corresponding
  files.
*/

typedef struct
{
  char *filenamebuf;    // pointer to a copy of a filename
  Uint filelength;      // the length of the corresponding file
} Fileinfo;             // \Typedef{Fileinfo}

/*
  The following is the type of the comparison function
  to be provided to the function \texttt{qsort}.
*/

typedef int (*Qsortcomparefunction)(const void *,const void *);

/*
  The following is the type of the function showing information in
  verbose mode.
*/

typedef void (*Showverbose)(char *);


//\IgnoreLatex{

#endif

//}
