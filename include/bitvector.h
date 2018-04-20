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
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz
 * For full source control tree, see https://github.com/lsund/wotd
 *
 */

#ifndef BITVECTOR_H
#define BITVECTOR_H
#include <limits.h>
#include "types.h"
#include "spaceman.h"

// Modulef for manipulating bitvectors, represented by an unsigned integer.

// Number of bits in in integer
#define INT_WORDSIZE (UintConst(1) << LOGWORDSIZE)

// The value represented by the most significant bit in the INT_WORDSIZE
// binary number, for example, INT_WORDSIZE = 64 on a 64 bit architecture.
// Since it is only the first bit, the value is 2 ^ (64 - 1)
#define FIRSTBIT\
        (UintConst(1) << (INT_WORDSIZE - 1))

// The value represented by the second most significant bit.
#define SECONDBIT\
        (FIRSTBIT >> 1)                   // \(010^{w-2}\)

#endif
