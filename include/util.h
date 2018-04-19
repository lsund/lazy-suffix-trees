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

#ifndef UTIL_H
#define UTIL_H

#include <string.h>

#include "types.h"
#include "init.h"
#include "eval.h"

void showstring(Uchar *left, Uchar *right);

void showpattern(Uchar *w, Uint wlen);

void printtime();

Uint randlen(Uint m_min, Uint m_max);

size_t strlenw(wchar_t *s);

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
