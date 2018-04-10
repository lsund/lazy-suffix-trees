#ifndef UTIL_H
#define UTIL_H

#include <string.h>

#include "types.h"
#include "init.h"
#include "eval.h"

extern Uchar *sentinel, *text, characters[UCHAR_MAX + 1];

extern Uint textlen, alphasize;

void showstring(Uchar *left, Uchar *right);

void showpattern(Uchar *w, Uint wlen);

void printtime();

Uint randlen(Uint m_min, Uint m_max);

// Macros for checking argument numbers

// Check if the number of arguments is exactly N
#define CHECKARGNUM(N,S)\
        if (argc != N)\
        {\
          fprintf(stderr,"Usage: %s %s\n",argv[0],S);\
          exit(EXIT_FAILURE);\
        }

// Read an integer from a string
#define PARSEINTARG(S)\
        if(sscanf(S,"%ld",&readint) != 1 || readint < 0)\
        {\
          fprintf(stderr,"invalid argument \"%s\": " \
                         "non-negative number expected\n",S);\
          exit(EXIT_FAILURE);\
        }

// Read a floating point value from a string
#define PARSEFLOATARG(S)\
        if(sscanf(S,"%f",&readfloat) != 1)\
        {\
          fprintf(stderr,"invalid argument \"%s\":"\
                         " floating point number expected\n",S);\
          exit(EXIT_FAILURE);\
        }

#endif
