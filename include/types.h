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


#ifndef TYPES_H
#define TYPES_H
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// These is the assumptions about the types
//
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

typedef wchar_t Wchar;

#define LOGWORDSIZE    6

#define UintConst(N)   (N##UL)
#define SintConst(N)   (N##L)

#endif
